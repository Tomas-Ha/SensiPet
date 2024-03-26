#include "states/state_thirsty.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites_thirsty.h"

const uint8_t * frames_thirsty[2] = {thirsty_f0, thirsty_f1};
int frame_thirsty_idx = 0;

void ThirstyState::init()
{
    frame_thirsty_idx = 0;
}

void ThirstyState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, frames_thirsty[frame_thirsty_idx], 48, 48, WHITE);
    gOled.display();
    frame_thirsty_idx++;
    if (frame_thirsty_idx >= 2) frame_thirsty_idx = 0;
}

void ThirstyState::cleanup()
{
    printf("Thirsty state cleaned up.\n");
}