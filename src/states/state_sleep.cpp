#include "states/state_sleep.h"
#include "screen/globals.h"
#include <cstdio>

void SleepState::init()
{
    printf("Initializing sleep state.\n");
    update(0);
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