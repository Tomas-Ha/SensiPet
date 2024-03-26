#include "states/state_eating.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites_eating.h"

#define NUM_EATING_FRAMES 6
const uint8_t * frames_eating[NUM_EATING_FRAMES] = {eating_f0, eating_f1, eating_f2, eating_f3, eating_f4, eating_f5};
int frames_eating_idx = 0;

void EatingState::init()
{
    update(0);
}

void EatingState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, frames_eating[frames_eating_idx], 48, 48, WHITE);
    gOled.display();
    frames_eating_idx++;
    if (frames_eating_idx >= NUM_EATING_FRAMES) frames_eating_idx = 0;
}

void EatingState::cleanup()
{
    printf("Eating state cleaned up.\n");
}