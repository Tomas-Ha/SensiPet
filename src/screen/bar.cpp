#include "screen/bar.hpp"

#include <cmath>
#include <algorithm>

void BarSprite::set_percentage(float val) {
    std::size_t threshold = ((std::size_t) std::round(img_bar_width * val / 100.0f)) * img_bar_height;
    std::size_t i = 0;
    std::transform(std::begin(img_bar), std::end(img_bar), std::begin(buf), [&i,threshold](std::uint8_t x){
        // return (std::uint8_t) 0;
        std::size_t bitidx = i * 8;

        // clamp nbits to be in [0, 8]
        std::size_t nbits = bitidx > threshold ? (bitidx > threshold + 8 ? 0 : bitidx - threshold) : 8;

        // mask out `nbits` bits 
        uint8_t mask = ((uint8_t) (((uint16_t)1) << nbits)) - ((uint8_t) 1);

        ++i;
        return x & mask;
    });
}

void BarSprite::render(Adafruit_SSD1306_I2c& renderer, std::size_t idx) {
    constexpr std::size_t x = 20;
    std::size_t y = 7 + idx * 18;

    renderer.drawBitmap(x, y, buf.data(), img_bar_width, img_bar_height, WHITE);
}
