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
#include <string>

// Setup button interrupts
InterruptIn button1(BUTTON1);


volatile bool button_pressed = false;
volatile float last_pressed = 0;

void button1_fall_handler()
{
    last_pressed = gSensiPet.get_eq()->tick();
}

void button1_rise_handler()
{
    if (gSensiPet.get_current_state()->name == "SCARED" || gSensiPet.get_current_state()->name == "EAT"|| gSensiPet.get_current_state()->name == "DRINK") return;
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

void init()
{
    init_microphone();

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