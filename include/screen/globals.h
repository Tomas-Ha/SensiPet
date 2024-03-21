#pragma once

#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

// an I2C sub-class that provides a constructed default
class I2CPreInit : public I2C
{
  public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
    {
        frequency(400000);
        start();
    };
};

extern I2CPreInit gI2C;
extern Adafruit_SSD1306_I2c gOled;

extern void clear_display();
extern void flip_display();