#pragma once

#include "sensipet.h"
#include "sensipet_state.h"

// Global states and state change wrapper functions
extern SensiPet gSensiPet;

inline void return_to_previous() 
{
    gSensiPet.update_previous_state();
}

inline void check_stats_state()
{
    gSensiPet.update_stats_state();
}