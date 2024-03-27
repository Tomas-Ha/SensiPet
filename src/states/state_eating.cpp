#include "states/state_eating.h"
#include "states/globals.h"
#include "screen/globals.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
#include <vector>
#include "sprites/sprites_eating.h"
#include "sprites/sprites_no.h"

#define NUM_EATING_FRAMES 6
const uint8_t * frames_eating[NUM_EATING_FRAMES] = {eating_f0, eating_f1, eating_f2, eating_f3, eating_f4, eating_f5};
int frames_eating_idx = 0;
bool eating_no = false;
const uint8_t *frames_eating_no[6] = {no_f0, no_f1, no_f0, no_f1, no_f0, no_f1};

void EatingState::init()
{
    frames_eating_idx = 0;
    eating_no = gSensiPet.get_hunger() == 100;
}

void EatingState::update(unsigned int delta_ms)
{
    if (frames_eating_idx >= NUM_EATING_FRAMES) {gSensiPet.get_eq()->call(check_stats_state); return;}
    gOled.clearDisplay();
    gOled.setTextColor(WHITE);
    display_stats();
    gOled.drawBitmap(70, 5, eating_no ? frames_eating_no[frames_eating_idx] : frames_eating[frames_eating_idx], 48, 48, WHITE);
    gOled.display();
    frames_eating_idx++;
    if (gSensiPet.get_hunger() + 5 <= 100) 
    {
        gSensiPet.set_hunger(gSensiPet.get_hunger() + 5);
    }
    else 
    {
        gSensiPet.set_hunger(100);
    }
    if (frames_eating_idx >= NUM_EATING_FRAMES && gSensiPet.get_thirst() - 15 >= 0) 
    {
        gSensiPet.set_thirst(gSensiPet.get_thirst() - 15);
    }
}

void EatingState::cleanup()
{
    printf("Eating state cleaned up.\n");
}