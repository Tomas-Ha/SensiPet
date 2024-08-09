# SensiPet
CSC385 – Microprocessor Systems Winter 2024 Course Project

**Demo Video:** [https://www.youtube.com/watch?v=a0KCShVmuzw](https://www.youtube.com/watch?v=a0KCShVmuzw)

## Description
A pocket companion that uses the sensors on the STM B-L475E-IOT1A Discovery Board as well as an SSD1306 OLED screen from DIYmalls and BLE to interact with the environment and other pets.

## File Navigation
This project consists of the following file structure:
- Library folders and `.lib` files:
  - `Adafruit_GFX`: This is the graphics driver for our SSD1306 OLED 128x64 pixel screen
  - `BLE_utils`: Contains a printer for BLE errors
  - BSP Drivers: Contains the *simple* drivers for the sensors on our board. We utilize the thermometer from this driver
  - `LSM6DSL`: Contains a more complex driver for the LSM6DSL sensor, or the accelerometer/gyroscope
  - `MP34DT01`: Contains the driver for the MP34DT01 MEMS microphone. It can be utilized for either microphone but we only use it for one.
  - `VL53L0X`: Contains the driver for the VL53L0X sensor, or the TOF sensor
  
- The `include` folder:
  - `action.h`: Contains an enum of actions that can be used to transition from one state to another
  - `ble`: Contains a `globals.h` header file that has global variables for ble
  - `microphone`: Contains a `globals.h` header file that has global variables and functions for the microphone
  - `screen`: Contains 2 files, a `globals.h` header file of global variables and functions, as well as a `bar.hpp` header file for the `BarSprite` class. This class is used to programmatically create/update and display a bar sprite for a SensiPet stat.
  - `sprites`: Contains several header files which contain the binary data for all of the sprite images.
  - `states`: Contains several header files for each state which defines override functions for the `init`, `update` (update frame), and `cleanup` state functions
  - `sensipet_state.h`: This is the header file for the abstract `SensiPetState` class which all the states extend
  - `sensipet_stats.h`: This header file is simply a `struct` that has the stats of the SensiPet
  - `sensipet.h`: This header file defines the global variables and methods of the SensiPet class
 
- The `src` folder:
  - `blep2p`: Contains `blep2p.cpp` which defines and implements all necessary functions for the `BluetoothP2P` class. This class is used to initialize and begin P2P ble comunication between two boards.
  - `microphone`: Contains `microphone.cpp` which utilizes the `MP34DT01` driver to implement all necessary functions (`init_microphone`, `start_recording`, and override functions) to record and process the microphone data.
  - `screen`: Contains `bar.cpp`, which implements the functionality necessary to update and display a bar sprite, and `screen.cpp`, which implements the global functions from `screen/globals.h`
  - `states`: Contains several state `cpp` files that implement the functions necessary for a `SensiPetState` subclass.
  - `main.cpp`: Initializes and begins the SensiPet program as well as creates and links all interrupt handlers. 
  - `sensipet_state.cpp`: Implements general methods utilized by all `SensiPetState` subclasses.
  - `sensipet.cpp`: Implements the core functionality of the Sensipet. Note, **the core of the functionality of our FSM is initialized and written in this file, and all other peripherals, such as ble, the microphone, and the screen, are initialized and enabled here.**
  
- `mbed_app.json`: Config file for the mbed app project
- `.png` files: Sprite images for the UI which were later converted

**In general, please begin with `main.cpp` and follow on to `sensipet.cpp`.**

## Setup and Running
First, clone this repository onto your computer into the `Mbed Programs` folder used by Mbed Studio, using the following command:
```
git clone https://github.com/Tomas-Ha/SensiPet.git
```
Then, ensure that the project has been added to mbed. Make sure that the `mbed-os` and all other sensor libraries have been installed. If not, refer to the *File Navigation* section above for a link to the drivers (these links should also be accessible within the `.lib` files). When installing Adafruit, be sure to set the width and height to 128 by 64 within the Adafruit library.

To set up the `SSD1306`, connect the SDA pin on the SSD1306 to the D14 pin on the B-L475E-IOT1A Discovery board, and the SCL pin on the SSD1306 to the D15 pin. Then connect the VCC pin to the 3.3V pin on the Discovery board, and the GND pin to the GND pin on the Discovery board.

Finally, flash the program from Mbed Studio onto the Discovery board. You should now see the SensiPet.
To feed the pet, press the user button 3 times quickly (no more than 300ms between each press).
To give the pet water, press the user button twice quickly.
To make the pet less lonely, hold your hand over the board (next to the NFC tag). You can also make the pet less lonely if you have a friend with another board as well. Simply hold the user button on both boards for **more than a second** and release. Then, hold the boards close together and watch the pets play!

If the pet has not been interacted with within the past 10 seconds, it will fall asleep. To wake it up, simply press the button or shake the board. But be careful, shaking the board (as well as making a loud noise), scares the pet and increases their loneliness.

Most importantly, have fun!
