#include <Arduino.h>
#include "DroneLib.h"

MotorDriver md;

void printxyz(int* xyz);
void printxyz();

void setup() {
  // put your setup code here, to run once:
  initialise();
  Accelorometer::initialise();

  //Accelorometer::calibrate(5);

  Accelorometer::clearoffsets();

  Serial.println("Before: ");
  printxyz(Accelorometer::getxyzraw());

  Accelorometer::calibrate();

  Serial.println("\nAfter: ");
  printxyz(Accelorometer::getxyzraw());
  delay(2000);




}

void printxyz()
{
  float *buff = Accelorometer::getxyz();
  Serial.print("X: ");
  Serial.print(buff[0]);
  Serial.print(" Y: ");
  Serial.print(buff[1]);
  Serial.print(" Z: ");
  Serial.println(buff[2]);
}

void printxyz(int* xyz)
{
  //float *buff = xyz;
  Serial.print("X: ");
  Serial.print(xyz[0]);
  Serial.print(" Y: ");
  Serial.print(xyz[1]);
  Serial.print(" Z: ");
  Serial.println(xyz[2]);
}

void loop() {
  // put your main code here, to run repeatedly:

  //delay(2000);
  printxyz(Accelorometer::getxyzraw());

}
