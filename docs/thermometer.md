# Thermometer Setup

Once the thermometer is assembled, programming it is fairly straightforward. You will need to install the [Arduino IDE](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE) and a [board manager](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) for the ESP32, however.

Once the IDE and board manager are installed, you can install the thermometer's software.

1. Open the `src/arduino` directory in the Arduino IDE
2. Rename `EXAMPLE.secrets.h` to `secrets.h` and replace the values in quotes with your information
3. Select `[Tools] -> [Board] -> [esp32] -> [`*`your model`*`]` to specify your compilation target
4. Connect the ESP32 to your computer using the USB port
5. Press the upload button (right arrow in the upper left of the IDE)
6. Wait until the IDE confirms that the upload was successful

After the software is uploaded to the ESP32, you can begin using the thermometer. It needs to be connected to 5V power through its USB port, but no longer needs to be connected to your computer.