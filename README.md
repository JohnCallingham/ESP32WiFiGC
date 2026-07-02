# ESP32WiFiGC

This component is part of a suite of components which can be used as part of a program which implements an OpenLCB/LCC node. It has been developed using PlatformIO and has been tested on an Arduino Nano ESP32.

There are two files, ESP32WiFiGC_V2.h is a replacement for the original file ESP32WiFiGC.h. The change was made as WiFi network selection is now perfomed by the LCC_CONFIGURATION component and not the WiFi_MULTI component as previously.

This component has the following dependency;-
- LCC_CONFIGURATION: https://github.com/JohnCallingham/LCC_CONFIGURATION

This is automatically downloaded when using PlatformIO.

It can be installed in PlatformIO by adding the following line to platform.ini;-

lib_deps = https://github.com/JohnCallingham/ESP32WiFiGC.git

The file ESP32WiFiGC_V2.h is a replacement for the file WiFiGC.h in the OpenLCB_Single_Thread library and has these main differences;-
- the WiFimanager code has been removed and instead uses the LCC_CONFIGURATION component to determine which WiFi network to connect to.
- all of the WiFi connection will take place to the main program loop so that disconnection and reconnection can be handled there.

