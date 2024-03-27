#include "mbed.h"
#include "sensipet.h"
#include "sensipet_state.h"
#include "screen/globals.h"
#include "microphone/globals.h"
#include "states/globals.h"
#include "ble/globals.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>

const char BluetoothP2P::DEVICE_NAME_PREFIX[] = "PET_";

/** Schedule processing of events from the BLE middleware in the event queue. */
void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
    gSensiPet.get_eq()->call(Callback<void()>(&context->ble, &BLE::processEvents));
}

BLE &bleInstance = BLE::Instance();
/* look for other device and then settle on a role */
BluetoothP2P bleP2p(bleInstance, gSensiPet.get_eq());


// Setup button interrupts
InterruptIn button1(BUTTON1);

volatile bool button_pressed = false;
volatile float last_pressed = 0;
volatile int num_pressed = 0;

void check_button() {
    float time = gSensiPet.get_eq()->tick();
    if (time - last_pressed >= 300) {
        if (num_pressed == 2) { gSensiPet.update_state(Action::BUTTON_DOUBLE); }
        else if (num_pressed >= 3) gSensiPet.update_state(Action::BUTTON_TRIPLE);
    }
}

void button1_fall_handler()
{
    float time = gSensiPet.get_eq()->tick();\
    if (time - last_pressed < 300) {
        num_pressed++;
    } else {
        num_pressed = 1;
    }
    last_pressed = time;
    gSensiPet.get_eq()->call_in(400ms, check_button);
}

void button1_rise_handler()
{
    float time = gSensiPet.get_eq()->tick();
    if (time - last_pressed > 1000) {
        gSensiPet.get_eq()->call([&](){bleP2p.run();});
    }
}

void init()
{
    init_microphone();

    // this will allow us to schedule ble events using our event queue
    bleInstance.onEventsToProcess(schedule_ble_events);

    // Setup button interrupts
    button1.fall(button1_fall_handler);
    button1.rise(button1_rise_handler);
}

int main()
{
    init();
    gSensiPet.start();
    return 0;
}