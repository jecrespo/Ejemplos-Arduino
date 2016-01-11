#include <dht11o22.h>

int PinSensor=8;
int PinSensor2=9;

void setup()
{
  
  Serial.begin(9600); //Iniciamos el puerto serie principal
  Serial.println("\n------------------------INICIO-------------------------");
}

void loop() {
  int chk, chk2;
  double temperatura, humedad, temperatura2, humedad2;
  int t;
  
  dht Sensor_DHT; //Variable en que se carga la lectura de los sensores
  dht Sensor_DHT2;
    
  chk=Sensor_DHT.read11_22(PinSensor);        //Lee el sensor
  chk2=Sensor_DHT2.read11_22(PinSensor2);        //Lee el sensor
  temperatura=Sensor_DHT.temperature;    
  humedad=Sensor_DHT.humidity;
  temperatura2=Sensor_DHT2.temperature;    
  humedad2=Sensor_DHT2.humidity;
  t=millis()/1000;
  
  Serial.print("----------");
  Serial.print(t);
  Serial.println(" segundos----------");

  Serial.print("Respuesta sensor: ");
  Serial.println(chk);
  
  for (int i=0; i<5; i++){
      Serial.print("byte");
      Serial.print(i);
      Serial.print(": ");
      int bite = Sensor_DHT.bits[i];
      Serial.print(bite);
      Serial.print(" ");
  }
  Serial.println("");
  Serial.print("Temperatura dht22: ");
  Serial.println(temperatura);
  
  Serial.print("Humedad dht22: ");
  Serial.println(humedad);
  
  Serial.print("\nRespuesta sensor2: ");
  Serial.println(chk2);
  
  
  for (int i=0; i<5; i++){
      Serial.print("byte");
      Serial.print(i);
      Serial.print(": ");
      int bite2 = Sensor_DHT2.bits[i];
      Serial.print(bite2);
      Serial.print(" ");
  }
   
  Serial.println("");
  Serial.print("Temperatura dht11: ");
  Serial.println(temperatura2);
  
  Serial.print("Humedad dht11: ");
  Serial.println(humedad2);
  delay(2000);
}

