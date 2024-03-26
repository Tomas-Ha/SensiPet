#include "states/state_lonely.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites_lonely.h"

#define NUM_LONELY_FRAMES 2
const uint8_t * frames_lonely[NUM_LONELY_FRAMES] = {lonely_f0, lonely_f1};
int frames_lonely_idx = 0;

void LonelyState::init()
{
    update(0);
}

void LonelyState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, frames_lonely[frames_lonely_idx], 48, 48, WHITE);
    gOled.display();
    frames_lonely_idx++;
    if (frames_lonely_idx >= NUM_LONELY_FRAMES) frames_lonely_idx = 0;
}

void LonelyState::cleanup()
{
    printf("Lonely state cleaned up.\n");
}