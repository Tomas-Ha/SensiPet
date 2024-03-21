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

// Setup button interrupts
InterruptIn button1(BUTTON1);

volatile bool button_pressed = false;

void button1_fall_handler()
{
    gSensiPet.update_state(Action::BUTTON_PRESSED);
}

void init()
{
    // TODO: Should be checking for errors here.
    init_microphone();

    setup_states();

    // Setup button interrupts
    button1.fall(button1_fall_handler);
}

int main()
{
    init();

    gSensiPet.set_current_state(&gMainState);

    // TODO: Fix this, there should be a method to using an event
    // queue without having to use a while loop.
    while (true)
    {
        gSensiPet.get_current_state()->get_event_queue()->dispatch_once();
    }

    return 0;
}