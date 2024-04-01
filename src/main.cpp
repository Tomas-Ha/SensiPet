#include "mbed.h"
#include "sensipet.h"
#include "sensipet_state.h"
#include "screen/globals.h"
#include "microphone/globals.h"
#include "states/globals.h"
#include "ble/globals.h"
#include "stm32l475e_iot01_tsensor.h"
#include "LSM6DSLSensor.h"
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

/// @brief check if the button has not been pressed in the past 300ms, 
/// if so, update the state based on the number of consecutive presses
void check_button() {
    float time = gSensiPet.get_eq()->tick();
    if (time - last_pressed >= 300) {
        if (num_pressed == 2) { gSensiPet.update_state(Action::BUTTON_DOUBLE); }
        else if (num_pressed >= 3) gSensiPet.update_state(Action::BUTTON_TRIPLE);
    }
}

/// @brief On fall, update the number of presses based on the last time pressed
void button1_fall_handler()
{
    // Wake up if sleeping
    if (gSensiPet.sleeping()) gSensiPet.wakeup();

    // Update num_pressed accordingly
    float time = gSensiPet.get_eq()->tick();\
    if (time - last_pressed < 300) {
        num_pressed++;
    } else {
        num_pressed = 1;
    }
    last_pressed = time;

    // Set up button press check
    gSensiPet.get_eq()->call_in(400ms, check_button);
}

/// @brief On rise, check if the button was held to initiate BLE
void button1_rise_handler()
{
    float time = gSensiPet.get_eq()->tick();
    if (time - last_pressed > 1000) {
        gSensiPet.get_eq()->call([&](){bleP2p.run();});
    }
}

/// @brief If asleep, the sensor should wake up and scare the pet
void sensor_irq_handler()
{
    if (gSensiPet.sleeping()) 
    { 
        gSensiPet.wakeup();
        if (gSensiPet.get_current_state()->name != "SCARED") gSensiPet.update_state(Action::SCARED);
    }
}

// Init accelerometer and gyro
static DevI2C lsm6dslI2c(PB_11,PB_10);
static LSM6DSLSensor lsm6dsl(&lsm6dslI2c, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, PD_11); 

void init()
{
    // Init Microphone and thermometer
    init_microphone();
    BSP_TSENSOR_Init();
    
    // Init and start accelerometer and gyro with an interrupt
    lsm6dsl.init(NULL);
    lsm6dsl.enable_x();
    lsm6dsl.enable_g();
    lsm6dsl.enable_single_tap_detection();
    lsm6dsl.enable_tilt_detection();
    lsm6dsl.attach_int1_irq(sensor_irq_handler);


    // use our event queue to schedule ble events
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