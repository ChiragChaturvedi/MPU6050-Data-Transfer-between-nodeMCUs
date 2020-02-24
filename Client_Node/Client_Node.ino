 #include <ESP8266WiFi.h>
  #include <WiFiUDP.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
String data;

#define     LED0        2         // WIFI Module LED
char*       ssid = "Your SSID";              // SERVER WIFI NAME
char*       password = "Your Password";          // SERVER PASSWORD
const String  Devicename = "D1";

  // WIFI Module Role & Port    
  IPAddress APlocal_IP(172,20,10,2);
  //IPAddress    apIP(10, 10, 10, 1);
  IPAddress APgateway(172,20,10,2);
  IPAddress APsubnet(255, 255, 255, 0);

  unsigned int UDPPort = 2390;      // local port to send and listen to  

  WiFiUDP Udp;

    char result[50];              // Buffer big enough for 7-character float
  char packetBuffer[255];  

void setup() {
s.begin(115200);
Serial.begin(115200);
  Check_WiFi_and_Connect();
}
 
void loop() {
  Send_Data_To_Server();

 
 
}

void Send_Data_To_Server() {
unsigned long tNow;
       s.write("s");
  if (s.available()>0)
  {
    data=s.readStringUntil('\n');
  }
  data = data + ", " + Devicename;                                            
 //dtostrf(data[0], 8, 0, result);                                 
data.toCharArray(result,50);
 
  Udp.beginPacket(APlocal_IP, UDPPort);                        
  // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());         
  Udp.write(result);
  Udp.endPacket();                                             

  while(1){
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remoteIp = Udp.remoteIP();
      Serial.print(remoteIp);
      Serial.print(", port ");
      Serial.println(Udp.remotePort());

      // read the packet into packetBufffer
      int len = Udp.read(packetBuffer, 255);
      if (len > 0) {
        packetBuffer[len] = 0;
      }
      Serial.print("Contents:");
      Serial.println(packetBuffer);
      break;                                                    // exit the while-loop
    }
    if((millis()-tNow)>1000){                                   // if more then 1 second no reply -> exit
      Serial.println("timeout");
      break;                                                    // exit
    }
  }
}

//====================================================================================

void Check_WiFi_and_Connect(){
  if (WiFi.status() != WL_CONNECTED){
    WiFi.disconnect();                                          // probably not necessary due to WiFi.status() != WL_CONNECTED
    WiFi.begin("DataTransfer");                                 // reconnect to the Network
    Serial.println();
    Serial.print("Wait for WiFi");

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP().toString());

    Udp.begin(UDPPort);
  }
}
