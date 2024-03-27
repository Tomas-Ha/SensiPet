#include "screen/globals.h"
#include "states/globals.h"
#include "sprites.h"
#include "microphone/globals.h"
#include "action.h"

// Global states
SensiPet gSensiPet;

int curr_frame = 0;
bool is_scared = false;
bool go_left = false;

void finish_state()
{
    gSensiPet.update_state(Action::FINISH);
}