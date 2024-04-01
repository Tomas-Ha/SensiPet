# SensiPet
CSC385 – Microprocessor Systems Winter 2024 Course Project

## Description
A pocket companion that uses the sensors on the STM B-L475E-IOT1A Discovery Board as well as an SSD1306 OLED screen from DIYmalls and BLE to interact with the environment and other pets.

## File Navigation
This project consists of the following file structure:
- Library folders and `.lib` files:
  - `Adafruit_GFX`:
  - `BLE_utils`:
  - BSP Drivers:
  - `LSM6DSL`:
  - `MP34DT01`:
  - `VL53L0X`:
- The `include` folder:
  - `action.h`
  - `ble`
  - `microphone`
  - `screen`
  - `sprites`
  - `states`
  - `sensipet_state.h`
  - `sensipet_stats.h`
  - `sensipet.h`
- The `src` folder:
  - `blep2p`
  - `microphone`
  - `screen`
  - `states`
  - `main.cpp`
  - `sensipet_state.cpp`
  - `sensipet.cpp`
- `mbed_app.json`:
- `.png` files:

## Setup and Running
First, clone this repository onto your computer into the `Mbed Programs` folder used by Mbed Studio, using the following command:
```git clone https://github.com/Tomas-Ha/BSP-Audio-dB-Recording.git```
Then, ensure that the project has been added to mbed. Make sure that the `mbed-os` and all other sensor libraries have been installed. If not, refer to the *File Navigation* section above for a link to the drivers (these links should also be accessible within the `.lib` files). When installing Adafruit, be sure to set the width and height to 128 by 64 within the Adafruit library.

To set up the `SSD1306`, connect the SDA pin on the SSD1306 to the D14 pin on the B-L475E-IOT1A Discovery board, and the SCL pin on the SSD1306 to the D15 pin. Then connect the VCC pin to the 3.3V pin on the Discovery board, and the GND pin to the GND pin on the Discovery board.

Finally, flash the program from Mbed Studio onto the Discovery board. You should now see the SensiPet.
To feed the pet, press the user button 3 times quickly (no more than 300ms between each press).
To give the pet water, press the user button twice quickly.
To make the pet less lonely, hold your hand over the board (next to the NFC tag). You can also make the pet less lonely if you have a friend with another board. Simply hold the user button on both boards for _more than a second_ and release. Then, hold the boards close together and watch the pets play!

Most importantly, have fun!
