#include<Wire.h>
#include <math.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ;		//Valores de aceleración
double angleYZ = 0;
double angleXZ = 0;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,6,true);  // request a total of 6 registers
  
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);

  angleYZ = atan((double)AcY / (double)AcZ);
  angleYZ = angleYZ*(57.2958);	//Pasar a grados
  
  angleXZ = atan((double)AcX / (double)AcZ);
  angleXZ = angleXZ*(57.2958);
  
  Serial.write("yz:");
  Serial.print(angleYZ);
  Serial.write("\n");
  
  Serial.write("xz:");
  Serial.print(angleXZ);
  Serial.write("\n");
  
  delay(1000);  
}