#include "DroneLib.h"
#include <Arduino.h>

void initialise()
{
  Wire.begin();
  Serial.begin(9600);
}

void transmit(int devaddr, int regaddr, int val)
{
  Wire.beginTransmission(devaddr);
  Wire.write(regaddr);
  Wire.write(val);
  Wire.endTransmission();
}

void transmit(int devaddr, int regaddr, int val, bool endcompletely)
{
  Wire.beginTransmission(devaddr);
  Wire.write(regaddr);
  Wire.write(val);
  Wire.endTransmission(endcompletely);
}

void moveheader(int devaddr, int regaddr)
{
  Wire.beginTransmission(devaddr);
  Wire.write(regaddr);
  Wire.endTransmission(false);
}

void moveheader(int devaddr, int regaddr, bool endcompletely)
{
  Wire.beginTransmission(devaddr);
  Wire.write(regaddr);
  Wire.endTransmission(endcompletely);
}


MotorDriver::MotorDriver(int speedpin1, int speedpin2)
{
  this->speedpin1 = speedpin1;
  this->speedpin2 = speedpin2;
  pinMode(speedpin1, OUTPUT);
  pinMode(speedpin2, OUTPUT);
}

MotorDriver::MotorDriver()
{
  this->speedpin1 = 5;
  this->speedpin2 = 6;
  pinMode(speedpin1, OUTPUT);
  pinMode(speedpin2, OUTPUT);
}

void MotorDriver::printpins()
{
  Serial.print("SpeedPin 1: ");
  Serial.print(this->speedpin1);
  Serial.print(" SpeedPin 2: ");
  Serial.println(this->speedpin2);
}

float MotorDriver::setmotorthrust(int pinindex, float percentage)
{
  int mapped = (percentage/100.0) * 256;
  Serial.println(mapped);
  analogWrite((pinindex)?speedpin2:speedpin1, mapped);
  return mapped;
}

void Accelorometer::initialise()
{
  Wire.beginTransmission(adxladr);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  //locoffset = new int[3];
}

float * Accelorometer::getxyz()
{
  int* raw = getxyzraw();

  static float tuple[3];
  tuple[0] = (raw[0] + locoffset[0])/256.0; tuple[1] = (raw[1] + locoffset[1])/256.0; tuple[2] = (raw[2] + locoffset[2])/256.0;



  return tuple;
}

int * Accelorometer::getxyzraw()
{
  Wire.beginTransmission(adxladr);
  Wire.write(dataxstart);
  Wire.endTransmission();
  Wire.requestFrom(adxladr, 6);
  int x = (( Wire.read()| Wire.read() << 8));
  int y = (( Wire.read()| Wire.read() << 8));
  int z = (( Wire.read()| Wire.read() << 8));

  static int tuple[3];
  tuple[0] = (x + locoffset[0]); tuple[1] = (y + locoffset[1]); tuple[2] = (z + locoffset[2]);



  return tuple;
}

/*int* Accelorometer::calibrate(int iterations)
{
  //set offset of the adxl to all 0 first, so that the raw values are actual rawvalues.
  Wire.beginTransmission(0x53);
  Wire.write(0x1E);
  Wire.write(0x00);
  Wire.write(0x1F);
  Wire.write(0x00);
  Wire.write(0x20);
  Wire.write(0x00);
  Wire.endTransmission();


  Serial.println("Calibration started...");
  int* rawvals[iterations];
  for (int i = 0; i < iterations; i++)
  {
    rawvals[i] = getxyzraw();
  }

  static float avgvals[3];

  for (int j = 0; j < 3; j++)
  {
    for (int i = 0; i < iterations; i++)
    {
      avgvals[j] = (avgvals[j] + rawvals[i][j]);
    }
    avgvals[j] = (avgvals[j]/iterations);
  }

  static int offset[3];
  offset[0] = (0 - avgvals[0])/4;
  offset[1] = (0 - avgvals[1])/4;
  offset[2] = (256 - avgvals[2])/4;

  Serial.println("Offset: ");
  Serial.print("X: ");
  Serial.print(offset[0]);
  Serial.print(" Y: ");
  Serial.print(offset[1]);
  Serial.print(" Z: ");
  Serial.println(offset[2]);
  Serial.println();



  Wire.beginTransmission(0x53);
  Wire.write(0x1E);
  Wire.write(offset[0]);
  Wire.write(0x1F);
  Wire.write(offset[1]);
  Wire.write(0x20);
  Wire.write(offset[2]);
  Wire.endTransmission();

  Serial.println("Original: ");
  Serial.print("X: ");
  Serial.print(avgvals[0]);
  Serial.print(" Y: ");
  Serial.print(avgvals[1]);
  Serial.print(" Z: ");
  Serial.println(avgvals[2]);
  Serial.println();

  int* current = getxyzraw();
  Serial.println("Current: ");
  Serial.print("X: ");
  Serial.print(current[0]);
  Serial.print(" Y: ");
  Serial.print(current[1]);
  Serial.print(" Z: ");
  Serial.println(current[2]);
  Serial.println();
  Serial.println();

  Serial.println("Calibration ended.");
  return offset;
}
*/

void Accelorometer::clearoffsets()
{
  transmit(adxladr, ofsx, 0x00);
  transmit(adxladr, ofsy, 0x00);
  transmit(adxladr, ofsz, 0x00);
  locoffset[0] = 0;
  locoffset[1] = 0;
  locoffset[2] = 0;
}


/*int* Accelorometer::calibrate(int iterations)
{
  Serial.println("Calibration started...");
  clearoffsets();
  int * current = getxyzraw();

  static int offsets[3];
  offsets[0] = (0 - current[0]);
  offsets[1] = (0 - current[1]);
  offsets[2] = (256 - current[2]);

  transmit(adxladr, ofsx, offsets[0]);
  transmit(adxladr, ofsy, offsets[1]);
  transmit(adxladr, ofsz, offsets[2]);

  Serial.println("Calibration ended.");
  return offsets;
}
*/

void Accelorometer::calibrate()
{
  clearoffsets();
  int* current = getxyzraw();
  //int off[3];
  locoffset[0] = (0 - current[0]);
  locoffset[1] = (0 - current[1]);
  locoffset[2] = (256 - current[2]);
  //locoffset = off;
}


int* Accelorometer::getoffset()
{
  Wire.beginTransmission(0x53);
  Wire.write(0x1E);
  Wire.endTransmission(false);
  Wire.requestFrom(0x53, 3);

  static int offsets[3];
  offsets[0] = Wire.read();
  offsets[1] = Wire.read();
  offsets[2] = Wire.read();

  return offsets;
}
