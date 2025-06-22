#ifndef ESP32WIFIGC_H
#define ESP32WIFIGC_H
/**
 * ESP32WifiGC.h
 * 
 * Created by John Callinghamn on 2025-05-06 from original code from WiFiGC.h
 *  which was created by Dave Harris on 2024-09-17.
 * Changes;-
 * - Removed WiFiManager code and used expliciy set AP ssid and password.
 * - Removed wifigc_init() from OlcbCanClass::init() to force wifigc_init() to only
 *    take place in main loop() and not in setup().
 */

#define NOCAN // disallow processor's CAN, this seems to fail on ESP32 compiler

#include "OlcbCan.h"

#include <WiFi.h>
#include <ESPmDNS.h>
WiFiClient client;
bool hubConnected = false;

class OlcbCanClass : public OlcbCan {
 public:
  OlcbCanClass(){}
  void init();
  uint8_t  avail();
  uint8_t  read();
  uint8_t  txReady();
  uint8_t  write(long timeout);
  uint8_t  write();
  uint8_t  close();
};

void wifigc_connectOpenLcb() {
  // Set up an MDNS responder.
  if (!MDNS.begin("ESP32_NodeNumber")) {
    Serial.printf("\n%6ld Error setting up MDNS responder!", millis());
    while(1){ delay(1000); }
  }

  // Find the Hub service.
  int n = MDNS.queryService(openLCB_can, "tcp");
  delay(1000);
  if (n == 0) {
    Serial.printf("\n%6ld Hub not available", millis());
    return;
  }

  /**
   * TO DO: If there is a previous hub connection then close it.
   * It seems that when rebooting the ESP32 we have to wait for
   *  any previous hub connection to time out before we can make another connection.
   * This appears to be an issue when in Dorset, but not in London!!
   */

  // Use the Hub service IP address to connect.
  for( int i=0; i<n; i++) {
    // Use WiFiClient class to create TCP connections
    // if (client.connect(MDNS.IP(i), 12021)) break;
    if (client.connect(MDNS.IP(i), 12021, 1000)) break; // Need to add the timeout parameter to work with JMRI 5.11.6 on laptop. 5.11.5 on RPi3B didn't need it !!
    Serial.printf("\n%6ld Hub connection Failed", millis());
    if(i==n) {
      i=0;
      delay(4000);
    }
  }
  
  Serial.printf("\n%6ld Connected to OpenLCB/LCC Hub", millis());
}

void wifigc_init() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFi.setMinSecurity(WIFI_AUTH_WPA_PSK); // Default is WPA2

  if( WiFi.status() != WL_CONNECTED ) {
    Serial.printf("\n%6ld Connecting to %s", millis(), ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(2000); // Originally 1000mS, but a second ESP32 didn't work until changed to 2000mS.
      Serial.printf("\n%6ld Connecting to %s", millis(), ssid);
    }
    Serial.printf("\n%6ld Connected to %s", millis(), ssid);
    Serial.printf("\n%6ld IP address: %s", millis(), WiFi.localIP().toString());
  }

  // wifigc_connectOpenLcb(); not required here as all initialisation happens in loop(), not setup().
}

// Returns true if hub connected, else false.
bool wifigc_process() {
  if (WiFi.status() != WL_CONNECTED) wifigc_init();
  if (!client.connected()) wifigc_connectOpenLcb();
  return client.connected();
}

// Returns true if the hub was not connected and is now connected, else false.
bool hubConnectionMade() {
  if (!hubConnected && wifigc_process()) {
    hubConnected = true;
    return true;
  }

  return false;
}

// Returns true if the hub was connected and is now disconnected, else false.
bool hubConnectionLost() {
  if (hubConnected && !wifigc_process()) {
    hubConnected = false;
    return true;
  }
  
  return false;
}

int readHex(char* b) {
  int t=0;
  if(b[0]>='0' && b[0]<='9') t += b[0]-'0';
  else if(b[0]>='A' && b[0]<='F') t += b[0]-'A'+10;
  else return -1;
  t *= 16;
  if(b[1]>='0' && b[1]<='9') t += b[1]-'0';
  else if(b[1]>='A' && b[1]<='F') t += b[1]-'A'+10;
  else return -1;
  return t;
}
int fromGC(OlcbCanClass *m, char* b) {
  int x;
  int p=0;
  while(b[p]!=':') { p++; }
  p++;
  if(b[p++]!='X') return -2;
  m->id = 0;
  for(int i=0; i<4; i++) {
    x = readHex(&b[p]); 
    if(x<0) break;
    m->id = m->id*256+x; 
    p+=2;
  }
  if(b[p++]!='N') return -3;
  m->length = 0;
  for(int i=0; i<8; i++) {
    m->data[i] = 0;
    x = readHex(&b[p]); 
    if(x<0) break;
    m->data[i] = m->data[i]*256 + x;
    m->length++;
    p += 2;
  }
  if(b[p]!=';') return -4;
  return 1;
}

int wifigc_read(OlcbCanClass *m) {
  enum State { sIDLE, sPACKET };
  static State state = sIDLE;
  static char buff[40];
  static int p;
  if(!client.available()) return 0;
  int c = client.read();
  if(state==sIDLE) {
    p = 0;
    if(c!=':') return 0;
    buff[p++]=':';
    state = sPACKET;
    return 0;
  } else {
    buff[p++] = c;
    if(c!=';') return 0;
    p = 0;
    state = sIDLE;
    //Serial.print(">>>"); Serial.print(buff);
    return fromGC(m, buff);
  }
}

#define GP(x) client.print(x)
#define GPH(x) client.print(x,HEX)
#define GP8(x) { if(x<16) GP(0); GPH(x); }
int wifigc_write(OlcbCanClass *m) {
  GP(":X");
  GP8((uint8_t)(m->id>>24));
  GP8((uint8_t)(m->id>>16));
  GP8((uint8_t)(m->id>>8));
  GP8((uint8_t)(m->id));
  GP("N");
  for(int i=0; i<m->length; i++) GP8(m->data[i]);
  GP(";\r\n");
  return 1;
}

  // void OlcbCanClass::init() { wifigc_init(); }
  void OlcbCanClass::init() {} // force wifigc_init() to only take place in main loop() and not in setup().
  uint8_t  OlcbCanClass::avail() { return 1; }
  uint8_t  OlcbCanClass::read() {
    //wifigc_process();  //// dph check this out
    return wifigc_read((OlcbCanClass*)this);
  }
  uint8_t  OlcbCanClass::txReady() { return 1; }
  uint8_t  OlcbCanClass::write(long timeout) { return wifigc_write((OlcbCanClass*)this); }
  uint8_t  OlcbCanClass::write() { return wifigc_write((OlcbCanClass*)this); }
  uint8_t  OlcbCanClass::close() { return 1; }

#endif // ESP32WIFIGC_H
