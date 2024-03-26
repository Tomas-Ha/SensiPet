#pragma once

#include "sensipet.h"
#include "sensipet_state.h"

// Global states
extern SensiPet gSensiPet;

inline void return_to_previous() 
{
    gSensiPet.update_previous_state();
}