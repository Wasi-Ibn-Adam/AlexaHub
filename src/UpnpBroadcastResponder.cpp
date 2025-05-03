#include "UpnpBroadcastResponder.h"
#include <functional>
 
// Multicast declarations
IPAddress ipMulti(239, 255, 255, 250);
const unsigned int portMulti = 1900;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

#define MAX_SWITCHES 14
AlexaSwitch switches[MAX_SWITCHES] = {};
int numOfSwitchs = 0;

//#define numOfSwitchs (sizeof(switches)/sizeof(Switch)) //array size  
 
//<<constructor>>
UpnpBroadcastResponder::UpnpBroadcastResponder(){
    
}
 
//<<destructor>>
UpnpBroadcastResponder::~UpnpBroadcastResponder(){/*nothing to destruct*/}
 
bool UpnpBroadcastResponder::init(){
  boolean state = false;
  
  Serial.println("Begin multicast ..");
  #if defined(ESP8266)
  if(UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti))
  #elif defined(ESP32)
  if(UDP.beginMulticast(ipMulti, portMulti))
  #endif
  {
    Serial.print("Udp multicast server started at ");
    Serial.print(ipMulti);
    Serial.print(":");
    Serial.println(portMulti);

    state = true;
  }
  else{
    Serial.println("Connection failed");
  }
  
  return state;
}

//Switch *ptrArray;

void UpnpBroadcastResponder::addDevice(AlexaSwitch& device) {
  Serial.print("Adding switch : ");
  Serial.print(device.getAlexaInvokeName());
  Serial.print(" index : ");
  Serial.println(numOfSwitchs);
  
  switches[numOfSwitchs] = device;
  numOfSwitchs++;
  device.init();
}

void UpnpBroadcastResponder::handle(){
  int packetSize = UDP.parsePacket();
  if (packetSize > 0){
    IPAddress senderIP = UDP.remoteIP();
    unsigned int senderPort = UDP.remotePort();
    
    // read the packet into the buffer
    UDP.read(packetBuffer, packetSize);
    
    // check if this is a M-SEARCH for WeMo device
    String request = String((char *)packetBuffer);

    if(request.indexOf("M-SEARCH") >= 0) {
        // Issue https://github.com/kakopappa/arduino-esp8266-alexa-multiple-wemo-switch/issues/22 fix
        // SWITCH TYPE REQUEST
        if((request.indexOf("urn:Belkin:device:**") > 0) || (request.indexOf("ssdp:all") > 0) || (request.indexOf("upnp:rootdevice") > 0)) {
          Serial.println("Got UDP Belkin Request..");
          
          // int arrSize = sizeof(switchs) / sizeof(Switch);
        
          for(int n = 0; n < numOfSwitchs; n++) {
              AlexaSwitch &sw = switches[n];

              if (&sw != NULL) {
                sw.respondToSearch(senderIP, senderPort);              
              }
          }
        }
      yield();
    }
  }

  for(int n = 0; n < numOfSwitchs; n++) {
    AlexaSwitch &sw = switches[n];
    if (&sw != NULL)
      sw.loop();
    yield();                       
  }
}
