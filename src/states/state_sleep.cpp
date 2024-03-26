#include "states/state_sleep.h"
#include "screen/globals.h"
#include <cstdio>

void SleepState::init()
{
    printf("Initializing sleep state.\n");
}

void SleepState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    gOled.setTextSize(2);
    gOled.setTextCursor(0, 0);
    gOled.printf("State: Sleep");
    gOled.display();
}

void SleepState::cleanup()
{
    printf("Cleaning up sleep state.\n");
}