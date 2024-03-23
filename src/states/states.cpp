#include "screen/globals.h"
#include "states/globals.h"
#include "sprites.h"
#include "microphone/globals.h"
#include "action.h"

// Global states
SensiPet gSensiPet;

// States
SensiPetState gMainState;
SensiPetState gScaredState;

int curr_frame = 0;
bool is_scared = false;
bool go_left = false;

void update_idle_frame() {
    if (gSensiPet.position.x + 48 >= 128) go_left = true;
    if (gSensiPet.position.x <= 0) go_left = false;
    gSensiPet.position.x += (go_left ? -2 : 2);
    curr_frame = 1 - curr_frame;
    gOled.drawBitmap(gSensiPet.position.x, gSensiPet.position.y, curr_frame == 0 ? idle_frame_1 : idle_frame_2, 48, 48, WHITE);
}

void main_state_update()
{
    gOled.clearDisplay();
    gOled.setTextCursor(0, 48);
    gOled.setTextSize(2);
    gOled.printf("State A");
    update_idle_frame();
    gOled.display();
}

void finish_state()
{
    gSensiPet.update_state(Action::FINISH);
}

void scared_state_update()
{
    gOled.clearDisplay();
    gOled.drawBitmap(gSensiPet.position.x, gSensiPet.position.y, scared_frame, 48, 48, WHITE);
    gOled.display();

    // Should transition back to main state once the animation has finished playing
    gSensiPet.get_current_state()->get_event_queue()->call_in(1000ms, finish_state);
}


void setup_states()
{
    gMainState.get_event_queue()->call_every(100ms, start_recording);
    gMainState.get_event_queue()->call_every(500ms, main_state_update);

    gScaredState.get_event_queue()->call_every(100ms, scared_state_update);
}