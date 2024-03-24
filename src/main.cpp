#include "mbed.h"
#include "sensipet.h"
#include "sensipet_state.h"
#include "screen/globals.h"
#include "microphone/globals.h"
#include "states/globals.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include "states/state_main.h"
#include "states/state_sleep.h"
#include "states/state_hungry.h"

// Setup button interrupts
InterruptIn button1(BUTTON1);

// States
MainState mainState;
SleepState sleepState;
HungryState hungryState;

volatile bool button_pressed = false;

void button1_fall_handler()
{
    gSensiPet.update_state(Action::BUTTON_PRESSED);
}

void setup_states()
{
    mainState.create_transition(Action::BUTTON_PRESSED, &hungryState);
    sleepState.create_transition(Action::BUTTON_PRESSED, &mainState);

    gSensiPet.set_current_state(&mainState);
}

void init()
{
    init_microphone();
    setup_states();

    // Setup button interrupts
    button1.fall(button1_fall_handler);
}

int main()
{
    init();
    gSensiPet.start();
    return 0;
}