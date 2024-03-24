#include "states/state_main.h"
#include "screen/globals.h"
#include <cstdio>

void MainState::init()
{
    printf("Main state initialized.\n");
    update(0);
}

void MainState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    gOled.setTextSize(2);
    gOled.setTextCursor(0, 0);
    gOled.printf("State: Main");
    gOled.display();
}

void MainState::cleanup()
{
    printf("Main state cleaned up.\n");
}