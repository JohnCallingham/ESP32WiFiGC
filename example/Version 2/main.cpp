...
#include "ESP32WiFiGC_V2.h"
#include "configurationPreferences.h"
...

void loop() {
...
  /**
   * Connect to the OpenLCB/LCC hub and reconnect if contact has been lost.
   */
  if (hubConnectionMade(ConfigurationPreferences::getWiFiSSID(), ConfigurationPreferences::getWiFiPassword())) {
    ledConfigHubConnected = true; // Turn the blue LED on if configured.

    // This is required so that JMRI is initialised if JMRI starts after the node has started.
    sendInitialEvents();
  }

  if (hubConnectionLost(ConfigurationPreferences::getWiFiSSID(), ConfigurationPreferences::getWiFiPassword())) {
    ledConfigHubConnected = false; // Turn the blue LED off if configured.
  }
...
}
