#include <Ethernet.h>
#include <SPI.h>

//const uint8_t chipSelect_SS_PIN = 4;   

byte mac[]     = {0x90, 0xA2, 0xDA, 0x0D, 0x01, 0xC9}; 
byte ip[]      = {192,168,1,169};   // Direccion IP de Arduino

EthernetServer server = EthernetServer(23);
String comando = "";

void setup() {
  Serial.begin(9600); 
  Serial.println("\n----------------------------INICIO---------------------------\n");

  Ethernet.begin(mac, ip); 
  server.begin();

}

void loop() {
  
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client == true) {
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    comando += client.read();
    if (comando == "quit") client.stop(); //si mando una q salgo del telnet
    server.println(comando);
}

  
}
