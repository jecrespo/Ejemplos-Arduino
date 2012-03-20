#include <Flash.h>
#include <SPI.h>
#include <Ethernet.h>

const uint8_t chipSelect_SS_PIN = 4;   

uint8_t resetCounter = 0;

static uint8_t mac[]     = {0x90, 0xA2, 0xDA, 0x00, 0x4A, 0x44}; 
static uint8_t ip[]      = {192,168,28,21};   // Direccion IP de Arduino
static uint8_t server[]  = { 77,238,184,40 }; // Mail server address  (this is for yahoo's mobile smtp)
//smtp.yahoo.com 587        77.238.184.40 ????
//smtp.mail.yahoo.com 587   216.145.54.171
//smtp.charter.net 25       209.225.8.224
//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1250101892/0
/*
auth login
c3lkbmV5c21zQHlhaG9vLmVz
YWRtaW5TTVM=
MAIL FROM: sydneysms@yahoo.es
*/
static Client YO_Cliente(server, 25);  // yahoo's mobile smtp server ip/port

void setup() {
  Serial.begin(9600); 
  Serial.println(F("\n----------------------------INICIO---------------------------\n"));

  Ethernet.begin(mac, ip); 
  Enviar_Correo();
}

void loop() {
}

void Enviar_Correo() {  
  Serial.println("Inicio de envío de correos:");                            // 77.238.184.40  25
  if (WEB_Conectar_con_Servidor())    {                                     // EHLO
    Serial.println("He conectado con el servidor");                         // auth login
                                                                            // c3lkbmV5c21zQHlhaG9vLmVz
    YO_Cliente.println("EHLO");                                             // YWRtaW5TTVM=
    Serial.print("\n     Digo: ");                                          // mail from: <sydneysms@yahoo.es>
    Serial.println("EHLO");                                                 // rcpt to: <japalacios@arsys.es>
    Serial.print("Respuesta: ");                                            // data
    WEB_Vaciar();                                                           // Subject: Asunto Message forwarding test.
                                                                            // Esto es lo que envío en el e-mail
    YO_Cliente.println("auth login");                                       // quit
    Serial.print("\n     Digo: ");
    Serial.println("auth login");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println("c3lkbmV5c21zQHlhaG9vLmVz");
    Serial.print("\n     Digo: ");
    Serial.println("c3lkbmV5c21zQHlhaG9vLmVz");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println("YWRtaW5TTVM=");
    Serial.print("\n     Digo: ");
    Serial.println("YWRtaW5TTVM=");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println("mail from: <sydneysms@yahoo.es>");
    Serial.print("\n     Digo: ");
    Serial.println("MAIL FROM: <sydneysms@yahoo.es>");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println("rcpt to: <japalacios@arsys.es>");
    Serial.print("\n     Digo: ");
    Serial.println("rcpt to: <japalacios@arsys.es>");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println("data");
    Serial.print("\n     Digo: ");
    Serial.println("data");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println("Subject: Message forwarding test.");
    Serial.print("\n     Digo: ");
    Serial.println("Subject: Message forwarding test.");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println("Esto es lo que envío en el e-mail");
    Serial.print("\n     Digo: ");
    Serial.println("Esto es lo que envío en el e-mail");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println(".");
    Serial.print("\n     Digo: ");
    Serial.println(".");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
    YO_Cliente.println("quit");
    Serial.print("\n     Digo: ");
    Serial.println("quit");
    Serial.print("Respuesta: ");
    WEB_Vaciar();
    
  }
  else Serial.println("FIN");
  resetCounter = 0;   // Contador de reintentos de conexión a 0
}

boolean WEB_Conectar_con_Servidor() {
 static boolean lastConnected = false;

 if (lastConnected  && !YO_Cliente.connected())  {   // Si estuvo conectado y ya no lo está
    #if USO_SERIE
      Serial.print(F("\n...pero estoy desconectado. ------> "));
    #endif
    YO_Cliente.stop();
  }
  
  INTENTO:
  if (YO_Cliente.connect())    { 
    #if USO_SERIE
      Serial.println(F("Connectando con el servidor...------> BIEN"));
    #endif  
    while (YO_Cliente.available()) {
      char c=YO_Cliente.read();     // Lee todo lo que haya llegado antes del servidor
      #if USO_SERIE
        Serial.print(c);
      #endif  
    }
    goto CONECTAR_HECHO;
  }
  else  { 
    #if USO_SERIE
      Serial.println(F("Conexion Fallida."));
    #endif
    resetCounter++;            
    if (resetCounter >=5 ) {   // Despues de 5 intentos de envio fallidos, reseteo la conexion con el servidor
      WEB_resetEthernetShield();   // y vuelvo a comprobar
      goto CONECTAR_HECHO;
    }    
  }
  goto INTENTO;
  
  CONECTAR_HECHO:
  lastConnected = YO_Cliente.connected(); 
  return lastConnected;
}

void WEB_resetEthernetShield()  {
  #if USO_SERIE
    Serial.println(F("Resetting Ethernet Shield.\n"));
  #endif
  YO_Cliente.stop();
  delay(100);
  Ethernet.begin(mac, ip); 
  delay(1000);
}

void WEB_Vaciar() {
  for (int i=0; i<1000; i++) if (YO_Cliente.available()) {
    char c=YO_Cliente.read();
    Serial.print(c);
  }
}

