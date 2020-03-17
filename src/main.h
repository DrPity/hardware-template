#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dhcp.h>
#include <ArduinoJson.h>

#define halt(s) { Serial.println(F( s )); while(1);  }