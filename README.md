
# RC_ESP32 by Rubem Pechansky

This ESP32 sketch uses the Bluepad32 library to receive signals from a standard Bluetooth game controller and forward them via the I2C bus.

It was primarily designed to be used with fischertechnik's TXT 4.0 controller, but it can easily be used with other hardware that support a 3.3V I2C bus. There is a [thread](https://forum.ftcommunity.de/viewtopic.php?f=8&t=8694) at the ftc forum with discussions and additional pictures.

![Custom-made ESP ft module](<images/ft_module.jpg>)

## What you'll need

Hardware:

- Any development board able to run Arduino Core for ESP32. (I chose the ESP32S NodeMCU ESP-12 because it fits neatly inside an ft 30 x 45 case.)
- Any Bluetooth game controller that supports Arduino mode (Mine is Gamesir Nova Lite)
- fischertechnik TXT 4.0

Additional parts to build the custom ft module:

- Shrouded 2x3-pin IDC box header, male
- 2 x shrouded 2×3-pin IDC connector, female
- 6-way ribbon cable
- 30 x 45 battery case bottom 32263
- Single-sided 30 x 30 perfboard

Notes:

- Another option to the battery case bottom 32263 is the bottom part of a ft sound module 130589, which is what I have used. In case you want to 3D-print the case yourself, the STL file available in the `files` folder may be useful.
- There are Fritzing files for the PCB and for breadboards in the `files` folder.

Development tools:

- [Arduino IDE 1.x](https://www.arduino.cc/en/software/#legacy-ide-18x) (version 2.3.6 has frequent problems with very high CPU usage)
- [Bluepad32 for Arduino + ESP32](https://bluepad32.readthedocs.io/en/latest/plat_arduino/#option-a-use-arduino-core-for-esp32-bluepad32-board)

## Connections

The connections between the MCU and the IDC connector should be the following:

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
- [Gamesir Nova Lite](https://www.amazon.com.br/Controlador-GameSir-Controle-Bluetooth-vibra%C3%A7%C3%A3o/dp/B0CMCQ6WMC?th=1) at Amazon
- [Gamesir NOVA Lite controller manual](https://cdn.shopify.com/s/files/1/2241/8433/files/Manual-GameSir_Nova_Lite.pdf)

## Troubleshooting

- The gamepad Home button (the "chicken") should be steady yellow (it's yellow-greeninsh actually) indicating it is in Android mode. If it is not, Press and hold Home + A until it does.
- To force pair the controller with the ESP32, press and hold Home + Screenshot (the button immediately below Home, with a small circle inside).
- If the ROBO Code program aborts, try resetting the ESP32 with a small screwdriver or similar tool, or simply disconnect and reconnect the I2C cable.
- There is a function called Detect_I2C_devices() in the ROBO Pro code RC_lib file. It will show all devices connected to the TXT 4.0 I2C port. It should display [0x18] which corresponds to I2C_SLAVE_ADDR in rc_esp32.ino.
