#include "sensipet.h"
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

// States
MainState mainState;
SleepState sleepState;
DrinkingState drinkingState("DRINK");
EatingState eatingState("EAT");
FriendshipState friendshipState;
HungryState hungryState;
LonelyState lonelyState;
ScaredState scaredState("SCARED");
ThirstyState thirstyState;

SensiPet::SensiPet()
{
    mainState.create_transition(Action::BUTTON_PRESSED, &hungryState);
    hungryState.create_transition(Action::BUTTON_PRESSED, &thirstyState);
    thirstyState.create_transition(Action::BUTTON_PRESSED, &lonelyState);
    hungryState.create_transition(Action::BUTTON_HELD, &eatingState);
    thirstyState.create_transition(Action::BUTTON_HELD, &drinkingState);
    lonelyState.create_transition(Action::BUTTON_PRESSED, &friendshipState);
    friendshipState.create_transition(Action::BUTTON_PRESSED, &sleepState);
    sleepState.create_transition(Action::BUTTON_PRESSED, &mainState);

    mainState.create_transition(Action::SCARED, &scaredState);
    hungryState.create_transition(Action::SCARED, &scaredState);
    thirstyState.create_transition(Action::SCARED, &scaredState);
    lonelyState.create_transition(Action::SCARED, &scaredState);
    sleepState.create_transition(Action::SCARED, &scaredState);

    mainState.create_transition(Action::FRIEND, &friendshipState);
    hungryState.create_transition(Action::FRIEND, &friendshipState);
    thirstyState.create_transition(Action::FRIEND, &friendshipState);
    lonelyState.create_transition(Action::FRIEND, &friendshipState);
    sleepState.create_transition(Action::FRIEND, &friendshipState);

    gSensiPet.set_current_state(&mainState);
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
    
    SensiPetState *next_state = current_state->get_transition(action);
    if (!next_state) return;
    
    // Cleanup current state
    current_state->cleanup();
    previous_state = current_state;

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

    set_current_state(previous_state);

    previous_state = current_state;
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

    previous_state = current_state;

    set_current_state(state);
}

void SensiPet::update_stats_state()
{
    if (this->get_thirst() <= 25) 
    {
        queue.call(queue.event(this, &SensiPet::update_stats_state_wrapper), &thirstyState);
    }
    else if (this->get_hunger() <= 25) 
    {
        queue.call(queue.event(this, &SensiPet::update_stats_state_wrapper), &hungryState);
    }
    else if (this->get_comfort() <= 25) 
    {
        queue.call(queue.event(this, &SensiPet::update_stats_state_wrapper), &lonelyState);
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
    current_state->update(0);
}

void SensiPet::start()
{
    // TODO: Change these around as you wish.
    queue.call_every(500ms, queue.event(this, &SensiPet::update_current_state));
    queue.call_every(100ms, start_recording);

    // Start the main event loop with an EventQueue
    while (true)
    {
        queue.dispatch_for(1000ms);
    }
}