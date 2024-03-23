#pragma once

#include "sensipet.h"
#include "sensipet_state.h"

// Global states
extern SensiPet gSensiPet;

// States
extern SensiPetState gMainState;
extern SensiPetState gScaredState;
extern SensiPetState gEatingState;

extern void setup_states();