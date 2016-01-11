void setup()
{
  
  Serial.begin(9600); //Iniciamos el puerto serie principal
  Serial.println("\n------------------------INICIO-------------------------");
}

int num_incoming_byte = 0;

void loop() {
  
  Serial.print("El numero de bytes entrantes es: ");
  num_incoming_byte = Serial.available();
  Serial.println(num_incoming_byte);
  if(num_incoming_byte >0)
  {
    char incoming_char=Serial.read();    // Tomamos el caracter que entra por el puerto serie
    Serial.println(incoming_char);         // Print el caracter al serial
    Serial.println(incoming_char, DEC);
  }
  delay(2000);
}

