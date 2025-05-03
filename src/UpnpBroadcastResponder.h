#ifndef UPNPBROADCASTRESPONDER_H
#define UPNPBROADCASTRESPONDER_H

 
#include <Arduino.h>
#if defined(ESP8266)
    #include <ESP8266WiFi.h>
    //#include <ESPAsyncTCP.h>
#elif defined(ESP32)
    #include <WiFi.h>
    //#include <AsyncTCP.h>
#endif
#include <WiFiUDP.h>
#include "AlexaSwitch.h"

#define UDP_TX_PACKET_MAX_SIZE 8192

class UpnpBroadcastResponder {
private:
        WiFiUDP UDP;  
public:
        UpnpBroadcastResponder();
        ~UpnpBroadcastResponder();
        bool init();
        void handle();
        void addDevice(AlexaSwitch& device);
};
 
#endif