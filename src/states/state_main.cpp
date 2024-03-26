#include "states/state_main.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites.h"

#define NUM_FRIEND_FRAMES 2
const uint8_t * frames_main[NUM_FRIEND_FRAMES] = {idle_frame_1, idle_frame_2};
int frames_main_idx = 0;

void MainState::init()
{
    update(0);
}

void MainState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, frames_main[frames_main_idx], 48, 48, WHITE);
    gOled.display();
    frames_main_idx++;
    if (frames_main_idx >= NUM_FRIEND_FRAMES) frames_main_idx = 0;
}

void MainState::cleanup()
{
    printf("Main state cleaned up.\n");
}