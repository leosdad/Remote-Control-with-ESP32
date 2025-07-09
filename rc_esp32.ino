//
// This ESP32 sketch uses the Bluepad32 library to receive signals from a standard Bluetooth
// game controller and forward them to a I2C bus.
//
// The code is based on the Bluepad32 library Controller example by Ricardo Quesada
//
// See README.MD for setup instructions and more information.
//

// TODO:
// - Make the gamepad reconnect automatically and quickly to this device when it is powered on
// - Add support for vibration commands received via I2C
// - Add support for more gamepad properties (e.g., battery level, etc.)

#pragma region Includes ----------------------------------------------------------------------------

#include <Bluepad32.h>
#include <Wire.h>

#pragma endregion

#pragma region Declarations ------------------------------------------------------------------------

// I2C slave address for this device.
#define I2C_SLAVE_ADDR 0x28

// Delay in milliseconds for the main loop.
#define LOOP_DELAY_MS 10

// Print messages to the console on device initialization and I2C setup.
#define PRINT_TO_CONSOLE true

// Continuously dump gamepad state to the console.
#define DUMP_TO_CONSOLE false

// Dump gyroscope and accelerometer state to the console. DUMP_TO_CONSOLE must also be true.
#define DUMP_CONSOLE_EXTRA false

// LED pin number (GPIO 2).
#define LED_PIN 2

// LED brightness level (0-255).
#define LED_BRIGHTNESS 80

// Interval in milliseconds for LED blinking.
#define LED_INTERVAL_MS 500

// Represents the state of a game controller.
struct GamepadState {
    uint8_t index;         // Controller index (8-bit)
    uint8_t dpad;          // D-pad (8-bit)
    uint16_t buttons;      // Bitmask of pressed buttons (16-bit)

    uint32_t axisX;        // (-511 - 512) left X axis (32-bit)
    uint32_t axisY;        // (-511 - 512) left Y axis (32-bit)
    uint32_t axisRX;       // (-511 - 512) right X axis (32-bit))
    uint32_t axisRY;       // (-511 - 512) right Y axis (32-bit))

    uint32_t brake;        // (0 - 1023): brake value (32-bit)
    uint32_t throttle;     // (0 - 1023): throttle value (32-bit)
    uint16_t miscButtons;  // Bitmask of pressed misc buttons (16-bit)

    uint32_t gyroX;        // Gyro X (32-bit)
    uint32_t gyroY;        // Gyro Y (32-bit)
    uint32_t gyroZ;        // Gyro Z (32-bit)
    uint32_t accelX;       // Accelerometer X (32-bit)
    uint32_t accelY;       // Accelerometer Y (32-bit)
    uint32_t accelZ;       // Accelerometer Z (32-bit)
} __attribute__((packed));

// Enum for LED states
enum class LedState {
    Off,
    Blinking,
    On
};

// Current LED state variable
LedState ledState = LedState::Blinking;

// LED state for blinking.
bool ledBlinkState = false;

// Last time the LED was toggled.
unsigned long lastToggle = 0;

// Array of connected controllers.
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Gamepad state instance. This will be sent via I2C to the master device.
GamepadState state;

#pragma endregion

#pragma region Callbacks ---------------------------------------------------------------------------

// Gets called any time a new gamepad is connected. Up to BP32_MAX_GAMEPADS controllers can be
// connected at the same time.
void onConnectedController(ControllerPtr ctl)
{
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
#if PRINT_TO_CONSOLE
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName(),
                properties.vendor_id, properties.product_id);
#endif
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }

    if (ctl->hasData()) {
        ledState = LedState::On;
    }

#if PRINT_TO_CONSOLE
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
#endif
}

// Gets called any time a gamepad is disconnected.
void onDisconnectedController(ControllerPtr ctl)
{
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
#if PRINT_TO_CONSOLE
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
#endif
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

#if PRINT_TO_CONSOLE
    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
#endif

    // Set the LED state to On if any controller is still connected, otherwise set it to Blinking.
    bool anyLeft = false;
    for (int j = 0; j < BP32_MAX_GAMEPADS; j++) {
        if (myControllers[j] && myControllers[j]->isConnected()) {
            anyLeft = true;
            break;
        }
    }
    ledState = anyLeft ? LedState::On : LedState::Blinking;
}

// This function is called when the master device requests data from this device.
void onRequest()
{
    Wire.write(reinterpret_cast<uint8_t*>(&state), sizeof(state));
}

// This function is called when the master device sends data to this device.
void onReceive(int numBytes)
{
    if(numBytes > 0) {

        // TODO: handle received data / vibration

        // command = Wire.read();
        // if(command == 'a') {
        //     // Do something
        // } else if(command == 'b') {
        //     // Do something else
        // }
    }
}

#pragma endregion

#pragma region Functions ---------------------------------------------------------------------------

// Dumps the gamepad state to the serial console for debugging purposes.
void dumpGamepad(ControllerPtr ctl)
{
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d"
        ", brake: %4d, throttle: %4d, misc: 0x%02x"
#if DUMP_CONSOLE_EXTRA
        ", gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d"
#endif
        "\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons()   // bitmask of pressed "misc" buttons
#if DUMP_CONSOLE_EXTRA
        ,
        ctl->gyroX(),        // Gyroscope X
        ctl->gyroY(),        // Gyroscope Y
        ctl->gyroZ(),        // Gyroscope Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
#endif
    );
}

// Processes the data for each gamepad and updates its current state.
void processGamepad(ControllerPtr ctl)
{
    state.index = ctl->index();             // Controller index (8-bit)
    state.dpad = ctl->dpad();               // D-pad (8-bit)
    state.buttons = ctl->buttons();         // bitmask of pressed buttons (16-bit)

    state.axisX = ctl->axisX();             // (-511 - 512) left X axis (32-bit)
    state.axisY = ctl->axisY();             // (-511 - 512) left Y axis (32-bit)
    state.axisRX = ctl->axisRX();           // (-511 - 512) right X axis (32-bit))
    state.axisRY = ctl->axisRY();           // (-511 - 512) right Y axis (32-bit))

    state.brake = ctl->brake();             // (0 - 1023): brake value (32-bit)
    state.throttle = ctl->throttle();       // (0 - 1023): throttle value (32-bit)
    state.miscButtons = ctl->miscButtons(); // bitmask of pressed misc buttons (16-bit)

    state.gyroX = ctl->gyroX();             // Gyro X (32-bit)
    state.gyroY = ctl->gyroY();             // Gyro Y (32-bit)
    state.gyroZ = ctl->gyroZ();             // Gyro Z (32-bit)
    state.accelX = ctl->accelX();           // Accelerometer X (32-bit)
    state.accelY = ctl->accelY();           // Accelerometer Y (32-bit)
    state.accelZ = ctl->accelZ();           // Accelerometer Z (32-bit)

#if DUMP_TO_CONSOLE
    dumpGamepad(ctl);
#endif
}

// Processes all connected controllers, assures each one is valid and connected,
// and updates their state.
void processControllers()
{
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            }
#if PRINT_TO_CONSOLE
            else {
                Serial.printf("Unsupported controller\n");
            }
#endif
        }
    }
}

// Blinks the LED at a predefined interval.
void blinkLED(unsigned long interval)
{
    unsigned long now = millis();

    if (now - lastToggle >= interval) {
        ledBlinkState = !ledBlinkState;
        analogWrite(LED_PIN, ledBlinkState ? LED_BRIGHTNESS : 0);
        lastToggle = now;
    }
}

#pragma endregion

// Arduino setup function. Runs in CPU 1
void setup()
{
    Serial.begin(115200);
    while (!Serial) {
        // Wait for serial port to connect. You don't have to do this in your game. This is only
        // for debugging purposes, so that you can see the output in the serial console.
        ;
    }

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);    // Turn the LED off (active low)

#if PRINT_TO_CONSOLE
    // Print firmware version and Bluetooth address
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n",
        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
#endif

    // This call is mandatory. It sets up Bluepad32 and creates the callbacks.
    BP32.setup(&onConnectedController, &onDisconnectedController);

    // Notice that scanning can be stopped / started at any time by calling:
    // BP32.enableNewBluetoothConnections(enabled);

    // "forgetBluetoothKeys()" should be called when the user performs a "device factory reset",
    // or similar. Calling "forgetBluetoothKeys" in setup() just as an example. Forgetting
    // Bluetooth keys prevents "paired" gamepads to reconnect. But it might also fix some
    // connection / re-connection issues.
    // BP32.forgetBluetoothKeys();

    // Enables the BLE Service in Bluepad32. This service allows clients, like a mobile app, to
    // setup and see the state of Bluepad32. By default, it is disabled.
    BP32.enableBLEService(false);

    // TODO: flash LED here

    // Start the I2C communication
    Wire.begin(I2C_SLAVE_ADDR);
    Wire.onRequest(onRequest);
    Wire.onReceive(onReceive);
}

// Arduino loop function. Runs in CPU 1.
void loop()
{
    // Update the LED state based on the current ledState variable.
    if(ledState == LedState::On) {
        analogWrite(LED_PIN, LED_BRIGHTNESS);
    } else if(ledState == LedState::Blinking) {
        blinkLED(LED_INTERVAL_MS);
    } else {
        digitalWrite(LED_PIN, LOW);
    }

    // This call fetches all the controllers' data. Call this function in your main loop.
    // The controllers' pointer (the ones received in the callbacks) gets updated automatically.
    bool dataUpdated = BP32.update();
    if(dataUpdated) {
        processControllers();
    }

    // The main loop must have some kind of "yield to lower priority task" event. Otherwise, the
    // watchdog will get triggered. If your main loop doesn't have one, just add a simple
    // vTaskDelay(1). Detailed info here: https://stackoverflow.com/questions/66278271/
    // vTaskDelay(LOOP_DELAY_MS);
    delay(LOOP_DELAY_MS);
}
