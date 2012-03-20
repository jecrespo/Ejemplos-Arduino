#include <dht11o22.h>

int PinSensor=8;

void setup()
{
  
  Serial.begin(9600); //Iniciamos el puerto serie principal
  Serial.println("\n------------------------INICIO-------------------------");
}

void loop() {
  int chk;
  double temperatura, humedad;

  dht Sensor_DHT; //Variable en que se carga la lectura de los sensores
    
  chk=Sensor_DHT.read11_22(PinSensor);        //Lee el sensor
  temperatura=Sensor_DHT.temperature;    
  humedad=Sensor_DHT.humidity;

  Serial.print("\nRespuesta sensor: ");
  Serial.println(chk);

  Serial.print("\nTemperatura: ");
  Serial.println(temperatura);
  
  Serial.print("Humedad: ");
  Serial.println(humedad);
  delay(2000);
}

