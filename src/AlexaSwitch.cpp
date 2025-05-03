#include "AlexaSwitch.h"
#include "templates.h"
        
//<<constructor>>
//AlexaSwitch::AlexaSwitch(){Serial.println("default constructor called");}
//Switch::Switch(String alexaInvokeName,unsigned int port){
AlexaSwitch::AlexaSwitch(String alexaInvokeName, unsigned int port, AlexaStateCallBack cb){
    String chipId = 
    #if defined(ESP8266)
      String(ESP.getChipId()); 
    #elif defined(ESP32)
      String((uint32_t)(ESP.getEfuseMac() >> 32));
    #endif

    int neededSize = snprintf(NULL, 0, ALEXA_SWITCH_UUID, chipId.c_str(), port) + 1; // Calculate required size
    char uuid[neededSize]; // Dynamically allocate buffer
    snprintf(uuid, neededSize, ALEXA_SWITCH_UUID, chipId.c_str(), port); // Safely format string
  
    persistent_uuid = String(uuid);
        
    device_name = alexaInvokeName;
    localPort = port;
    stateCallback = cb;
    //startWebServer();
}

//<<destructor>>
AlexaSwitch::~AlexaSwitch(){/*nothing to destruct*/}

void AlexaSwitch::loop(){
    if (server != NULL) {
        server->handleClient();
        delay(1);
    }
}

void AlexaSwitch::init(){
  #if defined(ESP8266)
    server = new ESP8266WebServer(localPort);
  #elif defined(ESP32)
    server = new WebServer(localPort);
  #endif
  server->on("/", [&]() { handleRoot(); });

  server->on("/setup.xml", [&]() { handleSetupXml(); });

  server->on("/upnp/control/basicevent1", [&]() { handleUpnpControl(); });

  server->on("/eventservice.xml", [&]() { handleEventservice(); });

  //server->onNotFound(handleNotFound);
  server->begin();

  Serial.println("WebServer started on port: ");
  Serial.println(localPort);
}
 
void AlexaSwitch::handleEventservice(){
  Serial.println(" ########## Responding to eventservice.xml ... ########\n");          
  server->send(200, "text/plain", ALEXA_SWITCH_EVENT_SERVICE);
}
 
void AlexaSwitch::handleUpnpControl(){
  Serial.println("########## Responding to  /upnp/control/basicevent1 ... ##########");      
  
  //for (int x=0; x <= HTTP.args(); x++) {
  //  Serial.println(HTTP.arg(x));
  //}

  String request = server->arg(0);      
  Serial.print("request:");
  Serial.println(request);
  

  if(request.indexOf("SetBinaryState") >= 0) {
    if(request.indexOf("<BinaryState>1</BinaryState>") >= 0) {
        Serial.println("Got Turn on request");
        if(stateCallback!=nullptr)
          if(stateCallback(true)) switchStatus = true;
        sendState();
    }

    if(request.indexOf("<BinaryState>0</BinaryState>") >= 0) {
        Serial.println("Got Turn off request");
        if(stateCallback!=nullptr)
          if(stateCallback(false)) switchStatus = false;
        sendState();
    }
  }

  if(request.indexOf("GetBinaryState") >= 0) {
    Serial.println("Got binary state request");
    sendState();
  }
  
  server->send(200, "text/plain", "");
}

void AlexaSwitch::handleRoot(){
  server->send(200, "text/plain", "You should tell Alexa to discover devices");
}

void AlexaSwitch::handleSetupXml(){
  Serial.println(" ########## Responding to setup.xml ... ########\n");

  int neededSize = snprintf(NULL, 0, ALEXA_SWITCH_SETUP_XML, device_name.c_str(), persistent_uuid.c_str(), switchStatus) + 1; // Calculate required size
  char responseBuffer[neededSize]; // Dynamically allocate buffer
  snprintf(responseBuffer, neededSize, ALEXA_SWITCH_SETUP_XML, device_name.c_str(), persistent_uuid.c_str(), switchStatus); // Safely format string
  server->send(200, "text/xml", responseBuffer);  // Send response
  
  Serial.print("Sending :");
  Serial.println(responseBuffer);
}

String AlexaSwitch::getAlexaInvokeName() { return device_name; }

void AlexaSwitch::setState(bool state) { switchStatus = state; }

void AlexaSwitch::sendState() {
  int neededSize = snprintf(NULL, 0, ALEXA_SWITCH_STATE, switchStatus) + 1; // Calculate required size
  char responseBuffer[neededSize]; // Dynamically allocate buffer
  snprintf(responseBuffer, neededSize, ALEXA_SWITCH_STATE, switchStatus); // Safely format string
  server->send(200, "text/xml", responseBuffer);  // Send response
  
  Serial.print("Sending :");
  Serial.println(responseBuffer);
}

void AlexaSwitch::respondToSearch(IPAddress& senderIP, unsigned int senderPort) {
  Serial.println("");
  Serial.print("Sending response to ");
  Serial.println(senderIP);
  Serial.print("Port : ");
  Serial.println(senderPort);


  String ip = WiFi.localIP().toString();

  int neededSize = snprintf(NULL, 0, ALEXA_SWITCH_SEARCH_RESPONSE, ip.c_str(), localPort, persistent_uuid.c_str()) + 1; // Calculate required size
  char responseBuffer[neededSize]; // Dynamically allocate buffer
  snprintf(responseBuffer, neededSize, ALEXA_SWITCH_SEARCH_RESPONSE, ip.c_str(), localPort, persistent_uuid.c_str()); // Safely format string

  UDP.beginPacket(senderIP, senderPort);
  #if defined(ESP8266)
    UDP.write(responseBuffer);
  #elif defined(ESP32)
    UDP.write((uint8_t* )responseBuffer, neededSize);
  #endif
  UDP.endPacket();
  /* add yield to fix UDP sending response. For more informations : https://www.tabsoverspaces.com/233359-udp-packets-not-sent-from-esp-8266-solved */
  yield();      

  Serial.println("Response sent !");
}