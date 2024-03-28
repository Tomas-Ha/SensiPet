#pragma once

#include <cstdint>
#include <array>

#include "sprites/sprites_ui.hpp"

class BarSprite {
    std::array<std::uint8_t, std::size(img_bar)> buf{img_bar};

public:

    BarSprite(const BarSprite& other) = delete;
    BarSprite& operator=(const BarSprite& other) = delete;
    BarSprite(BarSprite&& other) = delete;
    BarSprite& operator=(BarSprite&& other) = delete;

    constexpr BarSprite();


    void set_percentage(float val);
    void render(Adafruit_SSD1306_I2c& renderer, std::size_t idx);
};
