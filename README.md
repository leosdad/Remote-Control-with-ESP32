
# RC_ESP32 by Rubem Pechansky

This ESP32 sketch uses the Bluepad32 library to receive signals from a standard Bluetooth game controller and forward them via the I2C bus.

It was designed for use with fischertechnik. See the [discussion](https://forum.ftcommunity.de/viewtopic.php?f=8&t=8694) with pictures and a close-up of the PCB in the ftc forum.

![Custom-made ESP ft module](<images/ft_module.jpg>)

## Hardware and toolset

This software was successfully tested with the following hardware and software tools:

- ESP32S NodeMcu ESP-12 or any other board running Arduino Core for ESP32
- Gamesir Nova Lite or other game controller in Arduino mode
- fischertechnik TXT 4.0
- Arduino IDE 1.x (version 2.3.6 has frequent problems with very high CPU usage)
- For library installation, go [here](https://bluepad32.readthedocs.io/en/latest/plat_arduino/#option-a-use-arduino-core-for-esp32-bluepad32-board)

## Useful links

- [ESP32S NodeMcu ESP-12](https://www.makerhero.com/produto/modulo-wifi-esp32s-nodemcu-esp-12/) at MakerHero.com
- [ESP32S NodeMcu ESP-12 pinout](https://m.media-amazon.com/images/I/71LQk8thb7L.jpg)
- [Gamesir Nova Lite](https://www.amazon.com.br/Controlador-GameSir-Controle-Bluetooth-vibra%C3%A7%C3%A3o/dp/B0CMCQ6WMC?th=1) at Amazon
- [Gamesir NOVA Lite controller manual](https://cdn.shopify.com/s/files/1/2241/8433/files/Manual-GameSir_Nova_Lite.pdf)

## Troubleshooting

- The gamepad Home button (the "chicken") should be steady yellow (it's yellow-greeninsh actually) indicating it is in Android mode. If it is not, Press and hold Home + A until it does.
- To force pair the controller with the ESP32, press and hold Home + Screenshot (the button immediately below Home, with a small circle inside).
- If the ROBO Code program aborts, try resetting the ESP32 with a small screwdriver or similar tool, or simply disconnect and reconnect the I2C cable.
- There is a function called Detect_I2C_devices() in the ROBO Pro code RC_lib file. It will show all devices connected to the TXT 4.0 I2C port. It should display [0x18] which corresponds to I2C_SLAVE_ADDR in rc_esp32.ino.
