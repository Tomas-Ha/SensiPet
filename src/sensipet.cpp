#include "sensipet.h"

SensiPet::SensiPet()
{
}

SensiPet::~SensiPet()
{
}

// EventQueue wrappers
void SensiPet::init_current_state()
{
    current_state->init();
}

void SensiPet::update_current_state()
{
    unsigned int current_time = queue.tick();
    unsigned int elapsed_time = current_time - last_tick;
    current_state->update(elapsed_time);
}

void SensiPet::cleanup_current_state()
{
    current_state->cleanup();
}

void SensiPet::update_state_wrapper(Action action)
{
    if (!current_state) return;
    // Cleanup current state
    current_state->cleanup();

    SensiPetState *next_state = current_state->get_transition(action);
    if (!next_state) return;

    set_current_state(current_state->get_transition(action));
}

void SensiPet::update_state(Action action)
{
    // Defer this call to an event queue, since we may be calling it from an ISR.
    queue.call(queue.event(this, &SensiPet::update_state_wrapper), action);
}

SensiPetState *SensiPet::get_current_state()
{
    return current_state;
}

void SensiPet::set_current_state(SensiPetState *state)
{
    current_state = state;
    // Initialize the current state and begin updates
    current_state->init();
    current_state->update(0);
}

void SensiPet::start()
{
    // TODO: Change these around as you wish.
    queue.call_every(500ms, queue.event(this, &SensiPet::update_current_state));

    // Start the main event loop with an EventQueue
    while (true)
    {
        queue.dispatch_for(1000ms);
    }
}