# IoT Thermometer

This project contains everything necessary to create a custom IoT thermometer using an Arduino. This includes the thermometer's hardware design, a protective case for the thermometer, and software for the thermometer, the webpage the thermometer's data can be viewed from, and the server that powers both.


## Building/ Installation

Building this project is somewhat involved and takes place in several phases:

1. [Physical assembly](/docs/assembly.md)
2. [Programming the thermometer](/docs/thermometer.md)
> [!NOTE]
> You will need the website's URL in order to program the thermometer.
3. [Setting up the website](/docs/server.md)


## Usage

Once the thermometer is set up, use is very straightforward. 

1. Plug it in. It will automatically connect to your Wi-Fi network, alerting you once it has done so. It will then begin collecting temperature readings and publish them to the server. 
2. From an internet-connected device, browse to the web address you set the server up with. You will be able to view the current temperature, summary statistics of temperatures recorded, and a graph of the temperature over the last 24 hours.


## Coming Soon

More features will be added to the thermometer as time allows. Currently planned are: 

- Switch for activating/ deactivating the thermometer's Wi-Fi
- Wi-Fi captive portal to connect to the thermometer and provide it with the Wi-Fi credentials


## Pictures
![Thermometer](/docs/img/finished2.jpg)
![Website](/docs/img/website.png)