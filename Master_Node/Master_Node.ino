//-- Libraries Included --------------------------------------------------------------
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

//------------------------------------------------------------------------------------

  #define     LED0        2        

//------------------------------------------------------------------------------------

  char*       ssid = "Chirag's iPhone";              // SERVER WIFI NAME
  char*       password = "password1";          // SERVER PASSWORD

//------------------------------------------------------------------------------------
  #define     MAXSC     6        
    
  IPAddress APlocal_IP(172,20,10,2);
  IPAddress APgateway(172,20,10,2);
  IPAddress APsubnet(255, 255, 255, 0);

  unsigned int UDPPort = 2390;      

  WiFiUDP Udp;
//------------------------------------------------------------------------------------
 
  char packetBuffer[255];
  char result[10];


void setup() {
  Serial.begin(115200);

 
  SetWifi("DataTransfer","");
}

void loop() {

  HandleClients();

}



void HandleClients(){
unsigned long tNow;
  
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    //Serial.print("Received packet of size ");
    //Serial.println(packetSize);
    
    //Serial.print("From ");
    //Serial.print(Udp.remoteIP());
    //Serial.print(", port ");
    //Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    //Serial.println("Contents:");
    Serial.println(packetBuffer);
    
    tNow=millis();
    dtostrf(tNow, 8, 0, result);
    
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(result);
    Udp.endPacket();
  }
}


void SetWifi(char* Name, char* Password){
  
  WiFi.disconnect();


  WiFi.mode(WIFI_AP_STA);
  Serial.println("WIFI Mode : AccessPoint Station");

  ssid      = Name;
  password  = Password;

  // Starting the AccessPoint
  WiFi.softAPConfig(APlocal_IP, APgateway, APsubnet);                  
  WiFi.softAP(ssid, password, 1 ,0, MAXSC);                                                  
  Serial.println("WIFI < " + String(ssid) + " > ... Started");


  delay(50);

 
  IPAddress IP = WiFi.softAPIP();
   

  Serial.print("AccessPoint IP : ");
  Serial.println(IP);

  // Starting UDP Server
  Udp.begin(UDPPort);
  
  Serial.println("Server Started");
}
