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
    SensiPetState *previous_state = nullptr;
    EventQueue queue;

    void init_current_state();
    void update_current_state();
    void cleanup_current_state();

    void update_state_wrapper(Action action);
    void update_previous_state_wrapper();
    void update_stats_state_wrapper(SensiPetState *state);
    void check_sleep();
    void update_stats();

    void register_events();
    void remove_events();

    unsigned int last_state_change = 0;
    unsigned int last_tick = 0;

    bool is_sleeping = false;

    // Event IDs
    int microphone_event_id;
    int main_update_event_id;
    int sleep_check_event_id;

  public:
    SensiPet();
    ~SensiPet();
    
    Position position;

    void start();

    void update_state(Action action);
    void update_previous_state();
    void update_stats_state();
    SensiPetState *get_current_state();
    void set_current_state(SensiPetState *state);

    void wakeup();
    void sleep();

    inline bool sleeping() { return is_sleeping; }

    // Getters and setters for pet stats
    inline int16_t get_thirst()
    {
        return pet_stats.thirst;
    }
    inline int16_t get_hunger()
    {
        return pet_stats.hunger;
    }
    inline int16_t get_comfort()
    {
        return pet_stats.comfort;
    }
    inline EventQueue* get_eq()
    {
        return &queue;
    }

    inline void set_thirst(int16_t thirst)
    {
        pet_stats.thirst = thirst;
    }
    inline void set_hunger(int16_t hunger)
    {
        pet_stats.hunger = hunger;
    }
    inline void set_comfort(int16_t comfort)
    {
        pet_stats.comfort = comfort;
    }
};