#include "states/globals.h"
#include "screen/globals.h"
#include "ble/globals.h"
#include "screen/bar.hpp"
#include "sprites/sprites_ui.hpp"
#include "sprites.h"

#define SCREEN_POWER_PIN 

I2CPreInit gI2C(PB_9, PB_8);
Adafruit_SSD1306_I2c gOled(gI2C, LED1, 0x78, 64, 128);

BarSprite bars[3];

void clear_display()
{
    gOled.clearDisplay();
}

void flip_display()
{
    gOled.display();
}

/// @brief sets the percentage and displays the bars for all of the sensipet stats
void display_stats(){
    gOled.drawBitmap(0, 0, img_background.data(), img_background_width, img_background_height, WHITE);

    bars[0].set_percentage(gSensiPet.get_thirst());
    bars[1].set_percentage(gSensiPet.get_hunger());
    bars[2].set_percentage(100.0f - gSensiPet.get_comfort());

    for (std::size_t i = 0; i < ((std::size_t) 3); i++) {
        bars[i].render(gOled, i);
    }

    // Show bluetooth symbol if ble is running
    if (bleP2p.is_ble_running) {
        gOled.drawBitmap(125 - img_wifi_height, 0, img_wifi.data(), img_bar_width, img_wifi_height, WHITE);
    }
}