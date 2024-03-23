#pragma once

#include "sensipet_state.h"
#include "sensipet_stats.h"
#include <utility>

struct Position
{
    uint16_t x, y;
};

/// @brief Main class for handling the SensiPet
class SensiPet
{
  private:
    SensiPetStats pet_stats;
    SensiPetState *current_state = nullptr;

  public:
    SensiPet();
    ~SensiPet();
    
    Position position;

    void update_state(Action action);
    SensiPetState *get_current_state();
    void set_current_state(SensiPetState *state);

    // Getters and setters for pet stats
    inline uint16_t get_loneliness()
    {
        return pet_stats.loneliness;
    }
    inline uint16_t get_hunger()
    {
        return pet_stats.hunger;
    }
    inline uint16_t get_comfort()
    {
        return pet_stats.comfort;
    }

    inline void set_loneliness(uint16_t loneliness)
    {
        pet_stats.loneliness = loneliness;
    }
    inline void set_hunger(uint16_t hunger)
    {
        pet_stats.hunger = hunger;
    }
    inline void set_comfort(uint16_t comfort)
    {
        pet_stats.comfort = comfort;
    }
};