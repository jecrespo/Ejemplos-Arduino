/*  TITULO:

    AUTOR:
    
    DESCRIPCIÓN DEL PROGRAMA:
    
    ESQUEMA DE CONEXION:
    
                                      +-----+
         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |         GND/RST2  [ ][ ]            |
         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   SCL del módulo I2C
         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   SDA del módulo I2C
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ] |   
         | [ ]IOREF                 MISO/12[ ] |   
         | [ ]RST                   MOSI/11[ ]~|   
         | [ ]3V3    +---+               10[ ]~|   
         | [ ]5v    -| A |-               9[ ]~|   
         | [ ]GND   -| R |-               8[ ] |   
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[ ] |   
         |          -| I |-               6[ ]~|   
         | [ ]A0    -| N |-               5[ ]~|   
         | [ ]A1    -| O |-               4[ ] |   PA 
         | [ ]A2     +---+           INT1/3[ ]~|    
         | [ ]A3                     INT0/2[ ] |   
         | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   
         | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   
         |            [ ] [ ] [ ]              |
         |  UNO_R3    GND MOSI 5V  ____________/
          \_______________________/
   
*/

  // Importar librerías
    
  // Declaración de objetos

  // Variables
      
  // Funciones

  // Setup
  
  void setup() {
    // put your setup code here, to run once:

  }
  
  // Loop
  
  void loop() {  
    // put your main code here, to run repeatedly:

  }
