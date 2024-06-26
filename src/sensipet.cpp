#include "sensipet.h"
#include "globals.h"
#include "states/globals.h"
#include "microphone/globals.h"
#include "states/state_main.h"
#include "states/state_sleep.h"
#include "states/state_hungry.h"
#include "states/state_thirsty.h"
#include "states/state_drinking.h"
#include "states/state_eating.h"
#include "states/state_friendship.h"
#include "states/state_lonely.h"
#include "states/state_scared.h"
#include "screen/globals.h"
#include "stm32l475e_iot01_tsensor.h"
#include "VL53L0X.h"
#include "DevI2C.h"
#include <cstdint>
#include <cstdio>

#define TIME_UNTIL_SLEEP 10000

// States
MainState mainState;
DrinkingState drinkingState("DRINK");
EatingState eatingState("EAT");
FriendshipState friendshipState("FRIEND");
HungryState hungryState;
LonelyState lonelyState;
ScaredState scaredState("SCARED");
ThirstyState thirstyState;

static DevI2C vl53l0xI2c(PB_11,PB_10);
static DigitalOut shutdown_pin(PC_6);
static VL53L0X vl53l0x(&vl53l0xI2c, &shutdown_pin, PC_7);

SensiPet::SensiPet()
{
    // Init FSM transitions
    mainState.create_transition(Action::BUTTON_TRIPLE, &eatingState);
    hungryState.create_transition(Action::BUTTON_TRIPLE, &eatingState);
    lonelyState.create_transition(Action::BUTTON_TRIPLE, &eatingState);
    mainState.create_transition(Action::BUTTON_DOUBLE, &drinkingState);
    hungryState.create_transition(Action::BUTTON_DOUBLE, &drinkingState);
    thirstyState.create_transition(Action::BUTTON_DOUBLE, &drinkingState);
    lonelyState.create_transition(Action::BUTTON_DOUBLE, &drinkingState);

    mainState.create_transition(Action::SCARED, &scaredState);
    hungryState.create_transition(Action::SCARED, &scaredState);
    thirstyState.create_transition(Action::SCARED, &scaredState);
    lonelyState.create_transition(Action::SCARED, &scaredState);

    mainState.create_transition(Action::FRIEND, &friendshipState);
    hungryState.create_transition(Action::FRIEND, &friendshipState);
    thirstyState.create_transition(Action::FRIEND, &friendshipState);
    lonelyState.create_transition(Action::FRIEND, &friendshipState);
    eatingState.create_transition(Action::FRIEND, &friendshipState);
    drinkingState.create_transition(Action::FRIEND, &friendshipState);

    // Init Sensipet stats and state
    set_comfort(100);
    set_hunger(100);
    set_thirst(100);

    set_current_state(&mainState);
}

SensiPet::~SensiPet()
{
}

// EventQueue wrappers
void SensiPet::init_current_state()
{
    current_state->init();
}

void SensiPet::update_current_state()
{
    if (sleeping())
    {
        gOled.clearDisplay();
        gOled.display();
        return;
    }

    unsigned int current_time = queue.tick();
    unsigned int elapsed_time = current_time - last_tick;
    current_state->update(elapsed_time);
}

void SensiPet::cleanup_current_state()
{
    current_state->cleanup();
}

void SensiPet::update_state_wrapper(Action action)
{
    if (!current_state) return;
    
    // Get next state
    SensiPetState *next_state = current_state->get_transition(action);
    if (!next_state) return;
    
    // Cleanup and store current (now previous) state
    current_state->cleanup();
    previous_state = current_state;

    // Transition
    set_current_state(current_state->get_transition(action));
}

void SensiPet::update_state(Action action)
{
    // Defer this call to an event queue, since we may be calling it from an ISR.
    queue.call(queue.event(this, &SensiPet::update_state_wrapper), action);
}

void SensiPet::update_previous_state_wrapper()
{
    if (!current_state || !previous_state) return;
    // Cleanup current state
    current_state->cleanup();

    // Store current state in tmp
    SensiPetState *tmp = current_state;

    // Return to previous state
    set_current_state(previous_state);

    // Place old current state in previous_state
    previous_state = tmp;
}

void SensiPet::update_previous_state()
{
    // Defer this call to an event queue, since we may be calling it from an ISR.
    queue.call(queue.event(this, &SensiPet::update_previous_state_wrapper));
}

void SensiPet::update_stats_state_wrapper(SensiPetState *state)
{
    if (!state || !current_state) return;
    
    // Cleanup current state
    current_state->cleanup();

    // Store current state as previous state
    previous_state = current_state;

    // Transition to state
    set_current_state(state);
}

void SensiPet::update_stats_state()
{
    // Check thirst, then hunger, then comfort (loneliness). If any are below, update state to be corresponding state
    if (this->get_thirst() <= 25) 
    {
        if (current_state != &thirstyState) queue.call(queue.event(this, &SensiPet::update_stats_state_wrapper), &thirstyState);
    }
    else if (this->get_hunger() <= 25) 
    {
        if (current_state != &hungryState) queue.call(queue.event(this, &SensiPet::update_stats_state_wrapper), &hungryState);
    }
    else if (this->get_comfort() <= 25) 
    {
        if (current_state != &lonelyState) queue.call(queue.event(this, &SensiPet::update_stats_state_wrapper), &lonelyState);
    }
    else 
    {
        // Otherwise idle
        if (current_state != &mainState) queue.call(queue.event(this, &SensiPet::update_stats_state_wrapper), &mainState);
    }
}

SensiPetState *SensiPet::get_current_state()
{
    return current_state;
}

void SensiPet::set_current_state(SensiPetState *state)
{
    current_state = state;
    // Initialize the current state and begin updates
    current_state->init();

    if (sleeping()) return;
    
    current_state->update(0);
    last_state_change = queue.tick();
}

void SensiPet::update_stats()
{   
    // Don't update stats if we are in a state that modifies stats
    if (current_state->name == "EAT" || current_state->name == "DRINK" || current_state->name == "SCARED" || current_state->name == "FRIEND") return;
    
    // Degrade stats overtime. This should be called every 10 seconds or so.
    if (!is_sleeping) {
        // Degrade thirst based on temperature
        float curr_temp = BSP_TSENSOR_ReadTemp();
        set_thirst(get_thirst() - (int16_t)(curr_temp/10));

        // Decrease loneliness if petting the pet, otherwise increase loneliness
        uint32_t distance;
        int status = vl53l0x.get_distance(&distance);
        if (status == VL53L0X_ERROR_NONE && distance < 100 && distance > 0) {
            set_comfort(get_comfort() + 10);
            last_state_change = queue.tick();
        } else {
            set_comfort(get_comfort() - 2);
        }
        
    }
    else {
        // If sleeping, modify stats more slowly
        set_comfort(get_comfort() - 1);
        set_thirst(get_thirst() - 1);
    }
    set_hunger(get_hunger() - 1);

    // Ensure stats do not exceed bounds
    if (get_hunger() < 0) set_hunger(0);
    if (get_thirst() < 0) set_thirst(0);
    if (get_comfort() < 0) set_comfort(0);
    if (get_comfort() > 100) set_comfort(100);

    // Update the state accordingly
    queue.call(queue.event(this, &SensiPet::update_stats_state));
}

void SensiPet::check_sleep()
{
    // Check if no recent interraction. If so, we can sleep
    unsigned int current_time = queue.tick();
    if (current_time - last_state_change < TIME_UNTIL_SLEEP || bleP2p.is_ble_running) return;

    // Otherwise, we should try to fall asleep
    this->sleep();
}

void SensiPet::wakeup()
{
    last_state_change = queue.tick();
    register_events();
    is_sleeping = false;
}

void SensiPet::sleep()
{
    remove_events();
    gOled.clearDisplay();
    gOled.display();
    is_sleeping = true;
}

void SensiPet::register_events()
{
    main_update_event_id = queue.call_every(500ms, queue.event(this, &SensiPet::update_current_state));
    sleep_check_event_id = queue.call_every(5s, queue.event(this, &SensiPet::check_sleep));
    microphone_event_id = queue.call_every(100ms, start_recording);
}

void SensiPet::remove_events()
{
    queue.cancel(main_update_event_id);
    queue.cancel(sleep_check_event_id);
    queue.cancel(microphone_event_id);
}

void SensiPet::start()
{
    // Init TOF sensor
    vl53l0x.init_sensor(VL53L0X_DEFAULT_ADDRESS);

    // Update stats should always happen
    queue.call_every(2s, queue.event(this, &SensiPet::update_stats));

    register_events();

    // Start the main event loop with an EventQueue
    while (true)
    {
        queue.dispatch_for(1000ms);
    }
}