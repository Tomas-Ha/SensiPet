#include "screen/globals.h"
#include "states/globals.h"
#include "microphone/globals.h"
#include "action.h"

// Global states
SensiPet gSensiPet;

// States
SensiPetState gMainState;
SensiPetState gSecondaryState;
SensiPetState gThirdState;

void main_state_update()
{
    gOled.clearDisplay();
    gOled.setTextCursor(0, 0);
    gOled.setTextSize(2);
    gOled.printf("State A");
    update_idle_frame();
    gOled.display();
}

void secondary_state_update()
{
    gOled.clearDisplay();
    gOled.setTextCursor(0, 0);
    gOled.setTextSize(2);
    gOled.printf("State B");
    gOled.display();
}

void third_state_update()
{
    gOled.clearDisplay();
    gOled.setTextCursor(0, 0);
    gOled.setTextSize(2);
    gOled.printf("State C");
    gOled.display();
}

void setup_states()
{
    gMainState.get_event_queue()->call_every(100ms, start_recording);
    gMainState.get_event_queue()->call_every(500ms, main_state_update);

    gSecondaryState.get_event_queue()->call_every(500ms, secondary_state_update);

    gThirdState.get_event_queue()->call_every(500ms, third_state_update);

    gMainState.create_transition(Action::BUTTON_PRESSED, &gSecondaryState);
    gSecondaryState.create_transition(Action::BUTTON_PRESSED, &gThirdState);
    gThirdState.create_transition(Action::BUTTON_PRESSED, &gMainState);
}