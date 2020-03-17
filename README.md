# hardware-prototyping-template

### Template for easy hardware prototyping with MQTT

🔧🛠⚙️

## Usage

1. Grep one of our lovely **feather ethernet shields**
2. Install platformio
   `$ brew install platformio`
3. open the project and start writing code in the `main.cpp`

## In depth information

---

The **main.cpp**

It's the starting point of you program. Use it like a plain **.ino** file the only differnece is that it seperates _declarations_ which go into the `main.h` file from _definitions_ in the `main.cpp`

---

The **config.h**

Please define here all const variables for the preprocessor. They are all at one place, nice and tidy :sunglasses:

---

The **lib** folder

The `lib` folder contains all our custom libraries. Some nice **DEBUG** wrapper, a little **helpers** file and all the relevant stuff to handle network and MQTT in the **network** lib.

HAVE FUN

# Frequently occuring issues

## Compilation fails at the ArduinoJSON part

You might be running an old version of the ArduinoJson library. Remove the libraries with `rm -rf .piolibdeps` and reinstall them with `pio init`.

There were some breaking changes from ArduinoJson version 5 to version 6. Since PlatformIO does not allow for version
control when using the library IDs, we updated the hardware template to reflect the newest version. [For a detailed
breakdown of the changes have a look here](https://arduinojson.org/v6/doc/upgrade/)

## Bricked the Feather M0?

Sometimes you can brick your Feather M0 when you use the PlatformIO serial monitor and at the same time try to upload a sketch. The controller cannot be found
anymore although you are using the same usb-cable.
You can reactivate your Feather by following the [manual from Adafruit](https://learn.adafruit.com/adafruit-feather-32u4-adalogger/faq) (Ack! I did something... section):

- Open the arduino IDE and the basic blink example
- activated the `show verbose output` under the Arduino preferences with the upload setting
- while uploading, you see some `COM` ports printed out. Now **press the rest button _twice_** after which the LED should start to blink.
- If the upload was successful, the Feather should appear normally in the COM-port list again

The double press switches your Feather into bootloading mode.

## The wifi / ethernet connection does not work

Make sure you do not use any of the pins needed for the shields and change their pin mode.

_For ethernet_: pins 9 - 13

_For wifi_: pins 8,7,4,2

These are used for communication with the respective shields. Changing the pinmode on those might result in the connection not working properly.
