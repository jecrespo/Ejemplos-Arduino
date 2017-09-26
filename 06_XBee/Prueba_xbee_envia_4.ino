#include <LowPower.h>

//Prueba modulo XBEE programa recibe datos

//Joystick
//#define EJEY A5  //Joytick cable amarillo
//#define EJEX A4  //Jostick cable azul

//botones
#define BOTON1 7
#define BOTON2 6
#define BOTON3 5
#define BOTON4 4
#define wakeDownPin 3
#define wakeUpPin 2

int state = HIGH;
int estado = HIGH;

//int pos = 20;//100;  //punto muerto motores 
//int neg = -20;//-10;

//char mensaje;

void setup() {
  Serial.begin(9600);  
  attachInterrupt(0, wakeUp, RISING);
  pinMode(13,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(wakeDownPin, INPUT);   
  pinMode(wakeUpPin, INPUT); 
  pinMode(BOTON1, INPUT);
  pinMode(BOTON2, INPUT);
  pinMode(BOTON3, INPUT);
  pinMode(BOTON4, INPUT);
  
}

void loop() {
  Serial.println("Entrando en modo SLEEP");
  delay(100);
  digitalWrite(8, HIGH); 
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  Serial.println("Despertando");
  //digitalWrite(13,estado);  
  do{
    if(digitalRead(wakeUpPin) == HIGH)
      {
        digitalWrite(8, LOW); 
        delay(250); 
        Serial.write(77);
        delay(250); 
        digitalWrite(8, HIGH);
      } 
    if(digitalRead(BOTON1) == HIGH)
      {
        digitalWrite(8, LOW); 
        delay(250); 
        Serial.write(99);
        delay(250); 
        digitalWrite(8, HIGH);
      }
    if(digitalRead(BOTON2) == HIGH)
      {
        digitalWrite(8, LOW); 
        delay(250); 
        Serial.write(22);
        delay(250);  
        digitalWrite(8, HIGH);
      }
      if(digitalRead(BOTON3) == HIGH)
      {
        digitalWrite(8, LOW); 
        delay(250); 
        Serial.write(33);
        delay(250); 
        digitalWrite(8, HIGH);
      }
    if(digitalRead(BOTON4) == HIGH)
      {
        digitalWrite(8, LOW); 
        delay(250); 
        Serial.write(44);
        delay(250);  
        digitalWrite(8, HIGH);
      }
    }while(estado == LOW);
      digitalWrite(8, LOW); 
      delay(250); 
      Serial.write(66);
      delay(250); 
      digitalWrite(8, HIGH);
}

void wakeUp()
{
    // Just a handler for the pin interrupt.
    delay(300);
    estado=!estado;
    Serial.println("YEPA YEPA");
    //detachInterrupt(0); 
    delay(1200);
}
