#include "states/state_drinking.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites_drinking.h"

#define NUM_DRINKING_FRAMES 6
const uint8_t * frames_drinking[NUM_DRINKING_FRAMES] = {drinking_f0, drinking_f1, drinking_f2, drinking_f3, drinking_f4, drinking_f5};
int frames_drinking_idx = 0;

void DrinkingState::init()
{
    update(0);
}

void DrinkingState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, frames_drinking[frames_drinking_idx], 48, 48, WHITE);
    gOled.display();
    frames_drinking_idx++;
    if (frames_drinking_idx >= NUM_DRINKING_FRAMES) frames_drinking_idx = 0;
}

void DrinkingState::cleanup()
{
    printf("Drinking state cleaned up.\n");
}