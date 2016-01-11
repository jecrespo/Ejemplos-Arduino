/**
* Sonda de temperatura multiserver
*
* Enrique Crespo <jecrespom@gmail.com>
*
* 2012
*/

#include <Streaming.h>
#include <Ethernet.h>
#include <SPI.h>
#include "WebServer.h"
#include <MemoryFree.h>
#include <Agentuino.h> 
#include <Flash.h>
#include <dht11o22.h>
#include <SD.h>

int PinSensor=8;
int PinSensor2=9;
double temperatura, humedad;
int temperatura2, humedad2;

const int chipSelect = 4;

byte mac[]     = {0x90, 0xA2, 0xDA, 0x0D, 0x01, 0xC9}; 
byte ip[]      = {10,0,0,2};

EthernetServer telnet_server = EthernetServer(23);

#define PREFIX ""
WebServer webserver(PREFIX, 80);

void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  server.httpSuccess();
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
    P(helloMsg) = "<h1>Arduino Multiserver Sensor</h1>";
    char temp_web1[] = "<p>Temperatura sensor 1: ";
    char temp_web2[] = "<p>Temperatura sensor 2: ";
    char humed_web1[] = "<p>Humedad sensor 1: ";
    char humed_web2[] = "<p>Humedad sensor 2: ";

    /* this is a special form of print that outputs from PROGMEM */
    server.printP(helloMsg);
    server.print (temp_web1);
    server.print (temperatura);
    server.println ("</p>");
    server.print (humed_web1);
    server.print (humedad);
    server.println ("</p>");
    server.print (temp_web2);
    server.print (temperatura2);
    server.println ("</p>");
    server.print (humed_web2);
    server.print (humedad2);
    server.println ("</p>");
  }
}
  
#define DEBUG "TRUE"

//
// tkmib - linux mib browser
//
// RFC1213-MIB OIDs
// .iso (.1)
// .iso.org (.1.3)
// .iso.org.dod (.1.3.6)
// .iso.org.dod.internet (.1.3.6.1)
// .iso.org.dod.internet.mgmt (.1.3.6.1.2)
// .iso.org.dod.internet.mgmt.mib-2 (.1.3.6.1.2.1)
// .iso.org.dod.internet.mgmt.mib-2.system (.1.3.6.1.2.1.1)
// .iso.org.dod.internet.mgmt.mib-2.system.sysDescr (.1.3.6.1.2.1.1.1)
static char sysDescr[] PROGMEM      = "1.3.6.1.2.1.1.1.0";  // read-only  (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysObjectID (.1.3.6.1.2.1.1.2)
static char sysObjectID[] PROGMEM   = "1.3.6.1.2.1.1.2.0";  // read-only  (ObjectIdentifier)
// .iso.org.dod.internet.mgmt.mib-2.system.sysUpTime (.1.3.6.1.2.1.1.3)
static char sysUpTime[] PROGMEM     = "1.3.6.1.2.1.1.3.0";  // read-only  (TimeTicks)
// .iso.org.dod.internet.mgmt.mib-2.system.sysContact (.1.3.6.1.2.1.1.4)
static char sysContact[] PROGMEM    = "1.3.6.1.2.1.1.4.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysName (.1.3.6.1.2.1.1.5)
static char sysName[] PROGMEM       = "1.3.6.1.2.1.1.5.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysLocation (.1.3.6.1.2.1.1.6)
static char sysLocation[] PROGMEM   = "1.3.6.1.2.1.1.6.0";  // read-write (DisplayString)
// .iso.org.dod.internet.mgmt.mib-2.system.sysServices (.1.3.6.1.2.1.1.7)
static char sysServices[] PROGMEM   = "1.3.6.1.2.1.1.7.0";  // read-only  (Integer)
//
// Arduino defined OIDs
// .iso.org.dod.internet.private (.1.3.6.1.4)
// .iso.org.dod.internet.private.enterprises (.1.3.6.1.4.1)
// .iso.org.dod.internet.private.enterprises.arduino (.1.3.6.1.4.1.36582)
//
// .iso.org.dod.internet.private.enterprises.arduino.temp1 (.1.3.6.1.4.1.36582.1)
static char sysTemp1[] PROGMEM    = "1.3.6.1.4.1.36582.1.0";  // read-only  (Integer)
// .iso.org.dod.internet.private.enterprises.arduino.humed1 (.1.3.6.1.4.1.36582.2)
static char sysHumed1[] PROGMEM   = "1.3.6.1.4.1.36582.2.0";  // read-only  (Integer)
// .iso.org.dod.internet.private.enterprises.arduino.temp2 (.1.3.6.1.4.1.36582.3)
static char sysTemp2[] PROGMEM   = "1.3.6.1.4.1.36582.3.0";  // read-only  (Integer)
// .iso.org.dod.internet.private.enterprises.arduino.humed2 (.1.3.6.1.4.1.36582.4)
static char sysHumed2[] PROGMEM   = "1.3.6.1.4.1.36582.4.0";  // read-only  (Integer)
//
//
// RFC1213 local values
static char locDescr[]              = "Agentuino, a light-weight SNMP Agent.";  // read-only (static)
static char locObjectID[]           = "1.3.6.1.3.2009.0";                       // read-only (static)
static uint32_t locUpTime           = 0;                                        // read-only (static)
static char locContact[20]          = "Eric Gionet";                            // should be stored/read from EEPROM - read/write (not done for simplicity)
static char locName[20]             = "Agentuino";                              // should be stored/read from EEPROM - read/write (not done for simplicity)
static char locLocation[20]         = "Nova Scotia, CA";                        // should be stored/read from EEPROM - read/write (not done for simplicity)
static int32_t locServices          = 7;                                        // read-only (static)
static double locTemp1           = 0;                                        // read-only (static)
static double locHumed1           = 0;                                        // read-only (static)
static int locTemp2           = 0;                                        // read-only (static)
static int locHumed2           = 0;                                        // read-only (static)

uint32_t prevMillis = millis();
char oid[SNMP_MAX_OID_LEN];
SNMP_API_STAT_CODES api_status;
SNMP_ERR_CODES status;

void pduReceived()
{
  SNMP_PDU pdu;
  //
  #ifdef DEBUG
    Serial << F("UDP Packet Received Start..") << F(" RAM:") << freeMemory() << endl;
  #endif
  //
  api_status = Agentuino.requestPdu(&pdu);
  //
  if ( pdu.type == SNMP_PDU_GET || pdu.type == SNMP_PDU_GET_NEXT || pdu.type == SNMP_PDU_SET
    && pdu.error == SNMP_ERR_NO_ERROR && api_status == SNMP_API_STAT_SUCCESS ) {
    //
    pdu.OID.toString(oid);
    //
    //Serial << "OID: " << oid << endl;
    //
    if ( strcmp_P(oid, sysDescr ) == 0 ) {
      // handle sysDescr (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read-only
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        // response packet from get-request - locDescr
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locDescr);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //
      #ifdef DEBUG
        Serial << F("sysDescr...") << locDescr << F(" ") << pdu.VALUE.size << endl;
      #endif
    } else if ( strcmp_P(oid, sysUpTime ) == 0 ) {
      // handle sysName (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read-only
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        // response packet from get-request - locUpTime
        status = pdu.VALUE.encode(SNMP_SYNTAX_TIME_TICKS, locUpTime);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //
      #ifdef DEBUG
        Serial << F("sysUpTime...") << locUpTime << F(" ") << pdu.VALUE.size << endl;
      #endif
    } else if ( strcmp_P(oid, sysName ) == 0 ) {
      // handle sysName (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read/write
        status = pdu.VALUE.decode(locName, strlen(locName)); 
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        // response packet from get-request - locName
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locName);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //
      #ifdef DEBUG
        Serial << F("sysName...") << locName << F(" ") << pdu.VALUE.size << endl;
      #endif
    } else if ( strcmp_P(oid, sysContact ) == 0 ) {
      // handle sysContact (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read/write
        status = pdu.VALUE.decode(locContact, strlen(locContact)); 
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        // response packet from get-request - locContact
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locContact);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //
      #ifdef DEBUG
        Serial << F("sysContact...") << locContact << F(" ") << pdu.VALUE.size << endl;
      #endif
    } else if ( strcmp_P(oid, sysLocation ) == 0 ) {
      // handle sysLocation (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read/write
        status = pdu.VALUE.decode(locLocation, strlen(locLocation)); 
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      } else {
        // response packet from get-request - locLocation
        status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locLocation);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //
      #ifdef DEBUG
        Serial << F("sysLocation...") << locLocation << F(" ") << pdu.VALUE.size << endl;
      #endif
    } else if ( strcmp_P(oid, sysServices) == 0 ) {
      // handle sysServices (set/get) requests
      if ( pdu.type == SNMP_PDU_SET ) {
        // response packet from set-request - object is read-only
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = SNMP_ERR_READ_ONLY;
      } else {
        // response packet from get-request - locServices
        status = pdu.VALUE.encode(SNMP_SYNTAX_INT, locServices);
        pdu.type = SNMP_PDU_RESPONSE;
        pdu.error = status;
      }
      //
      #ifdef DEBUG
        Serial << F("locServices...") << locServices << F(" ") << pdu.VALUE.size << endl;
      #endif
    } else {
      // oid does not exist
      //
      // response packet - object not found
      pdu.type = SNMP_PDU_RESPONSE;
      pdu.error = SNMP_ERR_NO_SUCH_NAME;
    }
    //
    Agentuino.responsePdu(&pdu);
  }
  //
  Agentuino.freePdu(&pdu);
  //
  //Serial << "UDP Packet Received End.." << " RAM:" << freeMemory() << endl;
} 

void setup() {
  Serial.begin(9600); 
  Serial.println("\n----------------------------INICIO---------------------------\n");
  Serial.println("Pulsa cualquier tecla para ver temperatura y humedad");
  Serial.print("Initializing SD card...");
  
  pinMode(53, OUTPUT);
  if (!SD.begin(chipSelect)) {
     Serial.println("Card failed, or not present");
  }
  else {
   Serial.println("card initialized.");
  }

  Ethernet.begin(mac, ip); 
  telnet_server.begin();
  Serial.println("Telnet server initializated...");

  webserver.setDefaultCommand(&helloCmd);
  webserver.addCommand("index.html", &helloCmd);
  webserver.begin();
  Serial.println("Web server initializated...");
  
  //
  api_status = Agentuino.begin();
  //
  if ( api_status == SNMP_API_STAT_SUCCESS ) {
    //
    Agentuino.onPduReceive(pduReceived);
    //
    delay(10);
    //
    Serial << F("SNMP Agent Initalized...") << endl;
    //
    return;
  }
  //
  delay(10);
  //
  Serial << F("SNMP Agent Initalization Problem...") << status << endl;

}

void loop() {
  
  char buff[64];
  int len = 64;
  int save = 0;
  
  dht Sensor_DHT;
  dht Sensor_DHT2;
  
  Sensor_DHT.read11_22(PinSensor);
  Sensor_DHT2.read11_22(PinSensor2);
  temperatura=Sensor_DHT.temperature;    
  humedad=Sensor_DHT.humidity;
  temperatura2=Sensor_DHT2.temperature;    
  humedad2=Sensor_DHT2.humidity;
  
 
  
  EthernetClient telnet_client = telnet_server.available();
  if (telnet_client == true) {
    char telnet_read = telnet_client.read();
    
    while (telnet_client.available() > 0){
    telnet_client.read();
    }
        
    switch (telnet_read){
     case 'q':
     telnet_client.stop();
     break;
     case 't':
     telnet_server.println(temperatura);
     break;
     case 's':
     telnet_server.println(temperatura2);
     break;
     case 'h':
     telnet_server.println(humedad);
     break;
     case 'g':
     telnet_server.println(humedad2);
     break;
     default:
     telnet_server.println("pulsa t para ver temp1, s para ver temp2, h para ver humed1 y g para ver humed2");
     telnet_server.println("pulsa q para salir");
    }
        
}

  if (Serial.available() == true) {
    char serial_read = Serial.read();
    Serial.println("");
    Serial.print("Temperatura dht22: ");
    Serial.println(temperatura);
    Serial.print("Humedad dht22: ");
    Serial.println(humedad);
    Serial.println("");
    Serial.print("Temperatura dht11: ");
    Serial.println(temperatura2);
    Serial.print("Humedad dht11: ");
    Serial.println(humedad2);
  
}
   webserver.processConnection(buff, &len);
   
   Agentuino.listen();
   if ( millis() - prevMillis > 5000 ) {
    // increment previous milliseconds
    prevMillis += 5000;
    save = 1;
    // increment up-time counter
    locUpTime += 500;
    }
    
    locTemp1 = temperatura;
    locHumed1 = humedad;
    locTemp2 = temperatura2;
    locHumed2 = humedad2;
    
    File dataFile = SD.open("log_ms.txt", FILE_WRITE);
   
    if (dataFile && save) {
     dataFile.print(millis());
     dataFile.print(",");
     dataFile.print(temperatura);
     dataFile.print(",");
     dataFile.print(humedad);
     dataFile.print(",");
     dataFile.print(temperatura2);
     dataFile.print(",");
     dataFile.println(humedad2);
     dataFile.close();
     Serial.println("dato guardado en SD");
     }
     if (!dataFile && save) {
       Serial.println("error al guardar en SD");
     }
     
   
}
