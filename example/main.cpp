...
#include "ESP32WiFiGC.h"
 ...

void setup() {
...
  // Configure the built in blue LED and turn it off.
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);
}

void loop() {
...
  // Attempt to connect to the OpenLCB/LCC hub and reconnect if contact has been lost.
  if (hubConnectionMade()) {
    // Turn the blue LED on.
    digitalWrite(LED_BLUE, LOW);

    sendInitialEvents();
  }

  if (hubConnectionLost()) {
    // Turn the blue LED off.
    digitalWrite(LED_BLUE, HIGH);
  }
}
