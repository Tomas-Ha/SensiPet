#include "states/globals.h"
#include "screen/globals.h"
#include "sprites.h"

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