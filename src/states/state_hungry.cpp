#include "states/state_hungry.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites_hungry.h"

const uint8_t * frames[2] = {hungry_f0, hungry_f1};
int frame_idx = 0;

void HungryState::init()
{
    update(0);
}

void HungryState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, frames[frame_idx], 48, 48, WHITE);
    gOled.display();
    frame_idx++;
    if (frame_idx >= 2) frame_idx = 0;
}

void HungryState::cleanup()
{
    printf("Hungry state cleaned up.\n");
}