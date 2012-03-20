#include <Ethernet.h>
#include <SPI.h>

//const uint8_t chipSelect_SS_PIN = 4;   

byte mac[]     = {0x90, 0xA2, 0xDA, 0x0D, 0x01, 0xC9}; 
byte ip[]      = {10,0,0,2};   // Direccion IP de Arduino

EthernetServer server = EthernetServer(23);

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
    char lee = client.read();
    if (lee == 'q') client.stop(); //si mando una q salgo del telnet
    server.write(lee);
}

  
}
