#include "states/state_hungry.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites_hungry.h"

const uint8_t * frames_hungry[2] = {hungry_f0, hungry_f1};
int frames_hungry_idx = 0;

void HungryState::init()
{
    frames_hungry_idx = 0;
}

void HungryState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, frames_hungry[frames_hungry_idx], 48, 48, WHITE);
    gOled.display();
    frames_hungry_idx++;
    if (frames_hungry_idx >= 2) frames_hungry_idx = 0;
}

void HungryState::cleanup()
{
    printf("Hungry state cleaned up.\n");
}