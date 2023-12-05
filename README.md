# CryptoESP
Crypto environment for RSA keys generation with ESP32 module

**Mainly developped with ChatGPT**

# How it works
Launch **serialDial.py**, and communicate with the ESP32 via the serial connection
You can generate a private RSA key, hash in SHA256 or SHA512

# Building

In Arduino IDE, add **ESP32 Dev Module** Board
Install required librairies
Compile and export **CryptoESP.ino** to the ESP32

Launch **serialDial.py**, and you can communicate with the ESP32
You have to press Enter to get the result of the ESP32

# Required librairies

Seeed_Arduino_mbedtls **3.0.1**
Arduino.h
