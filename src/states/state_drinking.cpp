#include "states/state_drinking.h"
#include "states/globals.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites_drinking.h"
#include "sprites/sprites_no.h"

#define NUM_DRINKING_FRAMES 6
const uint8_t * frames_drinking[NUM_DRINKING_FRAMES] = {drinking_f0, drinking_f1, drinking_f2, drinking_f3, drinking_f4, drinking_f5};
int frames_drinking_idx = 0;
bool drinking_no = false;
const uint8_t *frames_drinking_no[6] = {no_f0, no_f1, no_f0, no_f1, no_f0, no_f1};

void DrinkingState::init()
{
    frames_drinking_idx = 0;
    drinking_no = gSensiPet.get_thirst() == 100;
}

void DrinkingState::update(unsigned int delta_ms)
{
    if (frames_drinking_idx >= NUM_DRINKING_FRAMES) {gSensiPet.get_eq()->call(return_to_previous); return;}
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, drinking_no ? frames_drinking_no[frames_drinking_idx] : frames_drinking[frames_drinking_idx], 48, 48, WHITE);
    gOled.display();
    frames_drinking_idx++;
    if (gSensiPet.get_thirst() + 5 <= 100) 
    {
        gSensiPet.set_thirst(gSensiPet.get_thirst() + 5);
    }
    else 
    {
        gSensiPet.set_thirst(100);
    }
}

void DrinkingState::cleanup()
{
    printf("Drinking state cleaned up.\n");
}