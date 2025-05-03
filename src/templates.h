#ifndef ALEXA_TEMP_H_
#define ALEXA_TEMP_H_

PROGMEM const char ALEXA_SWITCH_UUID[] = "Socket-1_0-38323636-4558-4dda-9188-cda0e6%s-%d";
PROGMEM const char ALEXA_SWITCH_EVENT_SERVICE[] =
      "<scpd xmlns=\"urn:Belkin:service-1-0\">"
        "<actionList>"
          "<action>"
            "<name>SetBinaryState</name>"
            "<argumentList>"
              "<argument>"
                "<retval/>"
                "<name>BinaryState</name>"
                "<relatedStateVariable>BinaryState</relatedStateVariable>"
                "<direction>in</direction>"
                "</argument>"
            "</argumentList>"
          "</action>"
          "<action>"
            "<name>GetBinaryState</name>"
            "<argumentList>"
              "<argument>"
                "<retval/>"
                "<name>BinaryState</name>"
                "<relatedStateVariable>BinaryState</relatedStateVariable>"
                "<direction>out</direction>"
                "</argument>"
            "</argumentList>"
          "</action>"
        "</actionList>"
        "<serviceStateTable>"
          "<stateVariable sendEvents=\"yes\">"
            "<name>BinaryState</name>"
            "<dataType>Boolean</dataType>"
            "<defaultValue>0</defaultValue>"
          "</stateVariable>"
          "<stateVariable sendEvents=\"yes\">"
              "<name>level</name>"
              "<dataType>string</dataType>"
              "<defaultValue>0</defaultValue>"
          "</stateVariable>"
        "</serviceStateTable>"
      "</scpd>\r\n"
      "\r\n";

PROGMEM const char ALEXA_SWITCH_SETUP_XML[] =
      "<?xml version=\"1.0\"?>"
        "<root>"
          "<device>"
            "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
            "<friendlyName> %s </friendlyName>"
            "<manufacturer>Belkin International Inc.</manufacturer>"
            "<modelName>Socket</modelName>"
            "<modelNumber>3.1415</modelNumber>"
            "<modelDescription>Belkin Plugin Socket 1.0</modelDescription>\r\n"
            "<UDN>uuid: %s </UDN>"
            "<serialNumber>221517K0101769</serialNumber>"
            "<binaryState>%d</binaryState>"
            "<serviceList>"
              "<service>"
                "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
                "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
                "<controlURL>/upnp/control/basicevent1</controlURL>"
                "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
                "<SCPDURL>/eventservice.xml</SCPDURL>"
              "</service>"
            "</serviceList>" 
          "</device>"
        "</root>\r\n"
      "\r\n";
PROGMEM const char ALEXA_SWITCH_STATE[] = 
      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
        "<u:GetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
          "<BinaryState>%d"
          "</BinaryState>\r\n"
        "</u:GetBinaryStateResponse>\r\n"
      "</s:Body> </s:Envelope>\r\n";

PROGMEM const char ALEXA_SWITCH_SEARCH_RESPONSE[] =  /*Uncomment the lines below and comment related lines if .... issues occur regarding device finding */
      "HTTP/1.1 200 OK\r\n"
      "CACHE-CONTROL: max-age=86400\r\n"
      "DATE: Sat, 26 Nov 2016 04:56:29 GMT\r\n"
      //"DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
      "EXT:\r\n"
      "LOCATION: http://%s:%d/setup.xml\r\n"
      "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
      "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
      "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
      "ST: urn:Belkin:device:**\r\n"
      //"ST: ssdp:all\r\n"
      "USN: uuid:%s::urn:Belkin:device:**\r\n"
      //"USN: uuid:%s::upnp:rootdevice\r\n"
      "X-User-Agent: redsonic\r\n\r\n";

      
      
      
      

#endif