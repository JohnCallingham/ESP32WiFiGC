# ESP32WiFiGC

## Update 14 June 2026

The file ESP32WiFiGC_V2.h is a replacement for the original file ESP32WiFiGC.h.

It removes the functionality of automatically connecting to one of many SSIDs
and replaces this with a user configurable SSID and password using the LCC_CONFIGURATION component.

## Original

The file ESP32WiFiGC.h is a replacement for the file WiFiGC.h in the OpenLCB_Single_Thread library.

It removes the WiFiManager code and instead uses the WiFi_MULTI library (https://github.com/JohnCallingham/WiFi_MULTI.git) to connect to one of a number of configured WiFi Access Points.

All of the WiFi connection will take place to the main program loop so that disconnection and reconnection can be handled there.

This library has been developed using PlatformIO.
