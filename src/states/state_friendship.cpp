#include "states/state_friendship.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites_friendship.h"

#define NUM_FRIEND_FRAMES 8
const uint8_t * frames_friendship[NUM_FRIEND_FRAMES] = {friendship_f1, friendship_f2, friendship_f3, friendship_f4, friendship_f5, friendship_f6, friendship_f7, friendship_f8};
int frames_friendship_idx = 0;

void FriendshipState::init()
{
    frames_friendship_idx = 0;
}

void FriendshipState::update(unsigned int delta_ms)
{
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    gOled.drawBitmap(0, 0, frames_friendship[frames_friendship_idx], 128, 64, WHITE);
    gOled.display();
    frames_friendship_idx++;
    if (frames_friendship_idx >= NUM_FRIEND_FRAMES) frames_friendship_idx = 0;
}

void FriendshipState::cleanup()
{
    printf("Friendship state cleaned up.\n");
}