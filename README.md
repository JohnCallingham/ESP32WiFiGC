# ESP32WiFiGC

This library has been designed to be part of the ESP32_2Servo_2Frog_2TOTI_WiFi program. It is part of a suite of libraries which can be used as part of an OpenLCB/LCC node.

This library has been developed using PlatformIO.

It has been designed to work with the OpenLCB_Single_Thread library (https://github.com/openlcb/OpenLCB_Single_Thread/tree/master)
and contains a single file ESP32WiFiGC.h which is a replacement for the original WiFiGC.h.

It can be installed in PlatformIO by updating platform.ini by editing lib_deps as below;

`lib_deps = openlcb/OpenLCB_Single_Thread@^0.1.12
          https://github.com/JohnCallingham/ESP32WiFiGC.git`

It can be installed into the Arduino IDE by downloading the .zip file and adding to the Arduino library using Sketch -> Include Library -> Add .Zip Library

