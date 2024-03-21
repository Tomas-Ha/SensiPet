#include "states/globals.h"
#include "microphone/globals.h"

// Global states
SensiPet gSensiPet;

// States
SensiPetState gMainState;

void setup_states()
{
    gMainState.get_event_queue()->call_every(100ms, start_recording);
    gMainState.get_event_queue()->call_every(500ms, update_idle_frame);
}