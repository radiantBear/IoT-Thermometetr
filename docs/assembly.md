# Thermometer Assembly

Assembling the thermometer is a somewhat involved process. You will need to 3D print a case, wire together the electronic components, and secure them within the case.

## Components

This project requires several physical components. These are:

1. 3D printer (access) for creating the thermometer's box
2. ESP32 Development Board
3. DS18B20 Temperature Probe
4. Sparkfun Basic 16x2 Character LCD
    - Other 16x2 displays should work with little trouble. The thermometer's box may need to be adjusted to the display's size, however.
5. Electronic connection components, eg:
    - Female-female DuPont connectors
    - Breadboard
    - Protoboard
    - Etc.
6. 3-pin SPDT slide switch 
    - Thermometer box may need to be adjusted to fit switches snugly

## 3D Printing

You will need to print both the [main thermometer box](/models/body.stl) and the [lid for the box](/models/top.stl). The main box should be printed with supports to ensure there is no droop at the top of the LCD screen slot.


## Electronic Assembly

You will need to connect all electronic components together so they can communicate. The ESP32 is the thermometer's "brain", and all other components connect to it. They should be connected as shown on the wiring schematic below. 

> **Warning:** Be sure to connect each component to the correct voltage level to avoid damage. If you changed any of the components, refer to their documentation for the expected voltage level. Pay special attention to the voltage expected by the ESP32's GPIO pins (eg for the switches).

> **Note:** Be sure to connect each of the ESP32's GPIO pins to the correct component pin to avoid communication issues.

![Wiring schematic](/docs/img/Wiring_Schematic.jpg)

### Wiring Examples
![Wiring on breadboard](/docs/img/breadboard.jpg)
![Wiring with DuPont connectors](/docs/img/esp32.jpg)

## Final Assembly

To complete thermometer assembly, place the wired-together electronics in the 3D printed case. 

The case is designed for the ESP32 to rest upside-down at the bottom with the USB port in the case wall's rectangular hole. 

The DS18B20 probe should stick out the circular hole on the other side of the case. The LCD screen should fit snugly in the hole near the case's top, and the switches may need to be glued in place to hold correctly.