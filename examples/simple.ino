#include <AlexaHub.h>

// prototypes , function can be replaced by lambda as well
bool onSwitch1Change(bool on);
bool onSwitch2Change(bool on);

// Change this before you flash
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PWSD";

boolean wifiConnected = false;

AlexaHub alexahub;

AlexaSwitch *myAlexaSwitch1 = NULL;
AlexaSwitch *myAlexaSwitch2 = NULL;

int mySwitchPin1 = 14;
int mySwitchPin2 = 13;

void setup() {
  Serial.begin(9600);
   
  // Initialise wifi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WIfi Connected: ");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  pinMode(mySwitchPin1, OUTPUT);
  pinMode(mySwitchPin2, OUTPUT);

  // we need make sure that our hub/board is initialized after that we will add switches in it
  alexahub.init();
    
  // Define your switches here. Max 10
  // Format: Alexa invocation name, local port no, stateCallback
  myAlexaSwitch1 = new AlexaSwitch("My Alexa Switch 1", 80, onSwitch1Change);
  myAlexaSwitch2 = new AlexaSwitch("My Alexa Switch 2", 81, onSwitch2Change);

  myAlexaSwitch1->setState(true);   // default value
  myAlexaSwitch2->setState(false);  // default value

  Serial.println("Adding switches in the alexa hub/board");
  alexahub.addDevice(*myAlexaSwitch1);
  alexahub.addDevice(*myAlexaSwitch2);
}
 
void loop(){ alexahub.handle(); }


bool onSwitch1Change(bool on){
  Serial.print("Switch 1 turn "); Serial.println(on?"on ... ":"off... ");
  digitalWrite(mySwitchPin1, on? HIGH: LOW);
  return true; // return true if state is handled, otherwise the change will not reflect on alexaButton
}
bool onSwitch2Change(bool on){
  Serial.print("Switch 2 turn "); Serial.println(on?"on ... ":"off... ");
  digitalWrite(mySwitchPin2, on? HIGH: LOW);
  return true; // return true if state is handled, otherwise the change will not reflect on alexaButton
}