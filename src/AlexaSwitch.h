#ifndef SWITCH_H
#define SWITCH_H
 
#include <Arduino.h>
//#include <ESP8266WiFi.h>
#if defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
   
    //#include <ESPAsyncTCP.h>
#elif defined(ESP32)
    #include <WiFi.h>
    #include <WebServer.h>
    //#include <AsyncTCP.h>
#endif
#include <WiFiUDP.h>
#include "CallbackFunction.h"

class AlexaSwitch {
private:
        #if defined(ESP8266)
                ESP8266WebServer *server = NULL;
                
        #elif defined(ESP32)
                WebServer *server = NULL;
        #endif
        WiFiUDP UDP;
        String persistent_uuid;
        String device_name;
        unsigned int localPort;

        AlexaStateCallBack stateCallback;
        bool switchStatus = false;
          
        void handleEventservice();
        void handleUpnpControl();
        void handleRoot();
        void handleSetupXml();
public:
        //AlexaSwitch();
        AlexaSwitch(String alexaInvokeName = "Switch", unsigned int port = 80, AlexaStateCallBack callback = nullptr);
        ~AlexaSwitch();

        void init();
        String getAlexaInvokeName();
        /*No need to call this function as Alexa Devs handle it*/
        void loop();
        /*No need to call this function as Alexa Devs handle it*/
        void respondToSearch(IPAddress& senderIP, unsigned int senderPort);
        void setState(bool state);
        void sendState();
};
 
#endif