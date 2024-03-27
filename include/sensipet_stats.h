#pragma once

#include <stdint.h>

/// @brief Struct to keep track of the SensiPet's current state. These values should be modified from
// within the SensiPet class instead of directly.
struct SensiPetStats
{
    // TODO: Unsure which data type we want to use for these stats. Should be unsigned though.
    int16_t thirst;
    int16_t hunger;
    int16_t comfort;
};