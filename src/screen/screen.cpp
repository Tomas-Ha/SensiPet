#include "states/globals.h"
#include "screen/globals.h"
#include "ble/globals.h"
#include "sprites.h"

#define SCREEN_POWER_PIN 

I2CPreInit gI2C(PB_9, PB_8);
Adafruit_SSD1306_I2c gOled(gI2C, LED1, 0x78, 64, 128);

void clear_display()
{
    gOled.clearDisplay();
}

void flip_display()
{
    gOled.display();
}

void display_stats(){
    gOled.setTextSize(1);
    gOled.setTextCursor(0, 0);
    if (bleP2p.is_ble_running) {
        gOled.printf("Thirst: %u%%\nHunger: %u%%\nLonely: %u%%\nScanning\r", gSensiPet.get_thirst(), gSensiPet.get_hunger(), gSensiPet.get_comfort());
    }
    else {
        gOled.printf("Thirst: %u%%\nHunger: %u%%\nLonely: %u%%\r", gSensiPet.get_thirst(), gSensiPet.get_hunger(), gSensiPet.get_comfort());
    }
}