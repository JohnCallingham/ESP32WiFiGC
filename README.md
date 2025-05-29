# ESP32WiFiGC
This is designed to work with the OpenLCB_Single_Thread library (https://github.com/openlcb/OpenLCB_Single_Thread/tree/master).

Contains a single file ESP32WiFiGC.h which is a replacement for the original WiFiGC.h.

It can be installed in PlatformIO by updating platform.ini by editing lib_deps as below;

`lib_deps = openlcb/OpenLCB_Single_Thread@^0.1.12
          https://github.com/JohnCallingham/ESP32WiFiGC.git`

It can be installed into the Arduino IDE by downloading the .zip file and adding to the Arduino library using Sketch -> Include Library -> Add .Zip Library

