# socketcar
A homemade remote controlled car. Uses a WebSocket protocol which allows the user to control from a website.\
![photo of the car](photo.jfif)

## Components
- NodeMCU ESP8266
- 2 Wheels with motors
- 2 Freewheel
- L298N Motor Driver
- A power source for the NodeMCU (something that can be connected to an USB Micro-B (the one you see at android chargers), a power bank for example)
- A power source for the Motor Driver (a battery)

## Setup

### Hardware
I highly recommend you to use a car frame like in the image above. It is nice and sturdy.\
1. The IN1, IN2, IN3, IN4 pins on the motor driver should be connected to the NodeMCU's D5, D6, D7, D7 pins.
2. The battery for the motor driver should be connected to the blue power supply plugs at the front of the motor driver (plug (+) to the left one and (-) to the middle one).
3. The NodeMCU's G (Ground) pin should also be connected to the middle power supply plug along with the battery's (-) cable.
4. And lastly, the two motors should be connected to the plugs at the left & right side of the motor driver.

### Software
1. Open up the code in your Arduino IDE.
2. Setup the IDE for handling NodeMCU.
    1. Go to File > Preferences.
    2. In the Additional Boards Manager URLs box, add http://arduino.esp8266.com/stable/package_esp8266com_index.json.
    3. Then go to Tools > Board > Boards Manager.
    4. Search for "esp8266 by ESP8266 Community" and install it.
    5. After that, go back to Tools > Board, then select NodeMCU 1.0.
3. Download [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip) & [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip)
4. Copy those files to your Arduino IDE libraries folder.
5. Or, go to Sketch > Include Library > Add .zip Library, and select the downloaded .zip files
