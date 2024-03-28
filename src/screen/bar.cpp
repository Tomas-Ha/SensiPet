#include "screen/bar.hpp"

#include <cmath>
#include <algorithm>

void BarSprite::set_percentage(float val) {
    std::size_t threshold = std::round(img_bar_width * val / 100.0f);
    std::size_t i = 0;
    std::transform(std::begin(img_bar), std::end(img_bar), std::begin(buf), [&i,=threshold](std::uint8_t x){
        std::size_t bitidx = i * 8;
        std::size_t x = bitidx % img_bar_width;

        // clamp nbits to be in [0, 8]
        std::size_t nbits = x > threshold ? (x > threshold + 8 ? 8 : x - threshold) : 0;

        // mask out `nbits` bits 
        uint8_t mask = ((uint8_t) (((uint16_t)1) << nbits)) - ((uint8_t) 1);
        
        // get the number of bits to the end of the row
        nbits = img_bar_width > x ? (img_bar_width > x + 8 ? 8 : img_bar_width - x) : 0;
        
        // mask out any bits in the next row 
        mask |= ~(((uint8_t) (((uint16_t)1) << nbits)) - ((uint8_t) 1));

        // one more masking operation to make sure, if the threshold is in the first 8 bits that the next
        // row is correct
        nbits = ((threshold < 8) && (x + 8 > threshold + img_bar_width)) ? 8 - ((x + 8) - (threshold + img_bar_width)) : 8;

        mask &= (((uint8_t) (((uint16_t)1) << nbits)) - ((uint8_t) 1));

        ++i;
        return x;
    });
}

void BarSprite::render(Adafruit_SSD1306_I2c& renderer, std::size_t idx) {
    constexpr std::size_t x = 20;
    std::size_t y = 7 + idx * 18;

    renderer.drawBitmap(x, y, buf, img_bar_width, img_bar_height, WHITE);
}
