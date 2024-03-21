#include "sensipet.h"

SensiPet::SensiPet()
{
}

SensiPet::~SensiPet()
{
}

void switch_event_queue(EventQueue* new_queue)
{
    
}

void SensiPet::update_state(Action action)
{
    if (!current_state) return;

    SensiPetState *next_state = current_state->get_transition(action);
    if (!next_state) return;

    set_current_state(current_state->get_transition(action));
}

SensiPetState *SensiPet::get_current_state()
{
    return current_state;
}

void SensiPet::set_current_state(SensiPetState *state)
{
    current_state = state;
}