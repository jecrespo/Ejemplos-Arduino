const int chipSelect = 4;
/*
 SD card read/write
  
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
  	 
 */

int PinSensor=8;
int PinSensor2=9;

int tiempo;
String dataString;

#include <SD.h>
#include <dht11o22.h>

void setup() {
  Serial.begin(9600);

  // On the Ethernet Shield, CS is pin 4. Note that even if it's not
  // used as the CS pin, the hardware CS pin (10 on most Arduino boards,
  // 53 on the Mega) must be left as an output or the SD library
  // functions will not work.
  
  Serial.print("Initializing SD card...");
   // make sure that the default chip select pin is set to
   // output, even if you don't use it:
   pinMode(53, OUTPUT);
 
 // see if the card is present and can be initialized:
   if (!SD.begin(chipSelect)) {
     Serial.println("Card failed, or not present");
     // don't do anything more:
     return;
   }
   Serial.println("card initialized.");
}

void loop() {
  
  // make a string for assembling the data to log:
  dataString = "";
  tiempo=millis()/1000;
  dataString += String(tiempo);
  dataString += ",";
  // read two sensors and append to the string:
  dht Sensor_DHT;
  dht Sensor_DHT2;
  Sensor_DHT.read11_22(PinSensor);        //Lee el sensor
  delay(1000);
  Sensor_DHT2.read11_22(PinSensor2);        //Lee el sensor
  delay(1000);
  int temperatura=Sensor_DHT.temperature_int;
  dataString += String(temperatura);
  dataString += ",";
  int humedad=Sensor_DHT.humidity_int;
  dataString += String(humedad);
  dataString += ",";
  int temperatura2=Sensor_DHT2.temperature_int;
  dataString += String(temperatura2);
  dataString += ","; 
  int humedad2=Sensor_DHT2.humidity_int;
  dataString += String(humedad2);
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
   File dataFile = SD.open("log.txt", FILE_WRITE);
 
  // if the file is available, write to it:
   if (dataFile) {
     dataFile.println(dataString);
     dataFile.close();
     // print to the serial port too:
     Serial.println(dataString);
   }  
   // if the file isn't open, pop up an error:
   else {
     Serial.println("error opening log.txt");
   } 
   
   delay(2000);

  
}


