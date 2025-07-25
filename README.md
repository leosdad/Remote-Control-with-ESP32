
# Remote Control with ESP32 by Rubem Pechansky

The primary reason I've built this module is the lack of a current physical remote control in the fischertechnik current lineup plus the fact that their controllers are unable to directly read the signals from a standard Bluetooth gamepad (as of 2024-2025, at least). It was primarily designed to be used with fischertechnik's TXT 4.0 controller, but it can easily be used with other hardware that has a 3.3 V I²C bus (not 5 V).

The ESP32 sketch included in this project uses the Bluepad32 library to receive signals from a standard Bluetooth game controller and forward them to a I²C bus. The example ROBO Pro Coding project included decodes these commands and uses them to control a model vehicle.

There is a [thread](https://forum.ftcommunity.de/viewtopic.php?f=8&t=8694) at the ftc forum with the original discussion, pictures of models, and very useful responses from other members.

![Custom-made ESP ft module](<images/ft_module.jpg>)

## What you'll need

Hardware:

- Any development board able to run Arduino Core for ESP32 (I chose the ESP32S NodeMCU ESP-12 because it fits neatly inside an ft 30 × 45 case.)
- Any Bluetooth game controller that supports Arduino mode (In my case, a GameSir T4 Nova Lite)
- A fischertechnik TXT 4.0 (or RX) controller

Parts to build the custom ft module:

- 2x3-pin IDC box header, male
- Two 2×3-pin IDC connectors, female
- 6-way ribbon cable
- 30 × 45 battery case bottom 32263, or the bottom part of a ft sound module 130589
- Single-sided 30 × 30 perfboard

Development tools:

- [Arduino IDE 1.x](https://www.arduino.cc/en/software/#legacy-ide-18x) *
- [Bluepad32 for Arduino + ESP32](https://bluepad32.readthedocs.io/en/latest/plat_arduino/#option-a-use-arduino-core-for-esp32-bluepad32-board)

\* I had frequent problems with very high CPU usage on version 2.3.6 so I went back to using the 1.x IDE. In any case, all code editing was done in Visual Studio Code anyway.

## Contents of the `files` folder

- The STL file included may be useful as a starting point in case you want to 3D-print an ft-compatible 30 × 45 case yourself.
- There are also Fritzing files for the PCB and for a breadboard.
- You may use the ROBO Pro Coding project included, `RC_vehicle_*.ft`, to build a test vehicle. The project includes the `RC_lib` and `Math_lib` libraries with Pyhton code created to ease the decoding of the gamepad inputs.
- I have included other useful images and the photo of one of the prototype vehicles I've built with the RC module.

## Connections

The connections between the MCU and the IDC connector are:

| MCU      | Pin # | IDC pin |
|----------|-------|---------|
| 3.3V     |  1    | 1       |
| GND      | 38    | 2       |
| SCL      | 36    | 5       |
| SDA      | 33    | 6       |

![EXT connector pinout](<images/ext_pinout.png>)

## Useful links

- [ESP32S NodeMCU ESP-12](https://www.makerhero.com/produto/modulo-wifi-esp32s-nodemcu-esp-12/) at MakerHero.com
- [ESP32S NodeMCU ESP-12 pinout](https://m.media-amazon.com/images/I/71LQk8thb7L.jpg)
- [GameSir T4 Nova Lite](https://www.amazon.com.br/Controlador-GameSir-Controle-Bluetooth-vibra%C3%A7%C3%A3o/dp/B0CMCQ6WMC?th=1) at Amazon
- [GameSir T4 Nova Lite controller manual](https://gamesir.com/pages/manuals-gamesir-t4n-lite)

## Troubleshooting

- The gamepad Home button (the "chicken") should be steady yellow-green indicating it is in Android mode. If it is not, Press and hold Home + A until it does.
- To force pair the controller with the ESP32, press and hold Home + Screenshot (the button immediately below Home, with a small circle inside).
- If the ROBO Code program aborts, try pressing the ESP32 with a small screwdriver or similar tool, or simply disconnect and reconnect the I²C cable.
- There is a function called Detect_I2C_devices() in the ROBO Pro code RC_lib file. It will show all devices connected to the TXT 4.0 I²C port. The address `0x28`, which corresponds to I2C_SLAVE_ADDR in `rc_esp32.ino`, should appear.
