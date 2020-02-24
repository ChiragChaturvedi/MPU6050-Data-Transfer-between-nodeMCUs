
#include <MPU6050_tockn.h>
#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial s(5,6);

float a[3];
String asd;
char asdf[50];

MPU6050 mpu6050(Wire);

void setup() {

  s.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();
  a[0] = mpu6050.getAngleX();
  a[1] = mpu6050.getAngleY();
  a[2] = mpu6050.getAngleZ();
  asd = String(a[0]) + "," + String(a[1]) + "," + String(a[2]) + '\n';
  asd.toCharArray(asdf,50);


  
  
 if(s.available()>0)
{
s.write(asdf);
 
}
  
}
