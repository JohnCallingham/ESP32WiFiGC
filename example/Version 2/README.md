This code was developed for an Arduino Nano ESP32 and shows part of main.cpp (or an .ino file) and indicates that the WiFi SSID and password are passed into ```hubConnectionMade()``` and ```hubConnectionLost()```.

It also shows how the board's blue LED is controlled as the JMRI hub is connected and disconnected.

It also allows code to be run when the hub is connected which may be used to send the current state of events to JMRI when the hub is first connected or is reconnected.
