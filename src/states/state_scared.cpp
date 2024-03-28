#include "states/state_scared.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites.h"

#define NUM_SCARED_FRAMES 2

void ScaredState::init()
{
    update(-1);
}

void ScaredState::update(unsigned int delta_ms)
{
    if (delta_ms > -1) return;
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, scared_frame, 48, 48, WHITE);
    gOled.display();
}

void ScaredState::cleanup()
{
    printf("Scared state cleaned up.\n");
}