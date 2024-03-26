#include "mbed.h"
#include "sensipet.h"
#include "sensipet_state.h"
#include "screen/globals.h"
#include "microphone/globals.h"
#include "state_thirsty.h"
#include "states/globals.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>
#include "states/state_main.h"
#include "states/state_sleep.h"
#include "states/state_hungry.h"
#include "states/state_drinking.h"
#include "states/state_eating.h"
#include "states/state_friendship.h"
#include "states/state_lonely.h"
#include "states/state_scared.h"

// Setup button interrupts
InterruptIn button1(BUTTON1);

// States
MainState mainState;
SleepState sleepState;
DrinkingState drinkingState;
EatingState eatingState;
FriendshipState friendshipState;
HungryState hungryState;
LonelyState lonelyState;
ScaredState scaredState("SCARED");
ThirstyState thirstyState;

volatile bool button_pressed = false;
volatile float last_pressed = 0;

void button1_fall_handler()
{
    last_pressed = gSensiPet.get_eq()->tick();
}

void button1_rise_handler()
{
    if (gSensiPet.get_current_state()->name == "SCARED" || gSensiPet.get_current_state() == &eatingState || gSensiPet.get_current_state() == &drinkingState) return;
    float time = gSensiPet.get_eq()->tick();
    gSensiPet.get_eq()->call(printf, "Time Diff: %f\n", time-last_pressed);
    if (time - last_pressed > 200) 
    {
        gSensiPet.update_state(Action::BUTTON_HELD);
    }
    else 
    {
        gSensiPet.update_state(Action::BUTTON_PRESSED);
    }
    
}

void setup_states()
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
    drinkingState.create_transition(Action::SCARED, &scaredState);
    eatingState.create_transition(Action::SCARED, &scaredState);
    lonelyState.create_transition(Action::SCARED, &scaredState);
    friendshipState.create_transition(Action::SCARED, &scaredState);
    sleepState.create_transition(Action::SCARED, &scaredState);

    gSensiPet.set_current_state(&mainState);
}

void init()
{
    init_microphone();
    setup_states();

    // Setup button interrupts
    button1.fall(button1_fall_handler);
    button1.rise(button1_rise_handler);
}

int main()
{
    init();
    gSensiPet.start();
    return 0;
}