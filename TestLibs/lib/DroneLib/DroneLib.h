#ifndef somevar
#define somevar

#include <Arduino.h>
#include "Wire.h"


static int adxladr = 0x53;
static int ofsx = 0x1E;
static int ofsy = 0x1F;
static int ofsz = 0x20;
static int dataxstart = 0x32;

//setup of basic Serial and wire and stuff
void initialise();

//transmits; by default endcompletely is true;
void transmit(int devaddr, int regaddr, int val);
void transmit(int devaddr, int regaddr, int val, bool endcompletely);

//by default this DOES NOT end completely
void moveheader(int devaddr, int regaddr);
void moveheader(int devaddr, int regaddr, bool endcompletely);


//Obviously only 1 pin per motor, and a motordriver can handle 2 pins, so 2 instances of this.
//Again, the direction control pins will be tied to high and low manually so it shouldn't be a problem.
//Also, speedpins HAVE to be PWMs
class MotorDriver
{
public:
  MotorDriver(int speedpin1, int speedpin2);
  MotorDriver();

  //sets motor thrust to a percentage of the maximum thrust
  float setmotorthrust(int pinindex, float percentage);

  //prints pins to serial
  void printpins();
private:
  int speedpin1, speedpin2;
};


static int locoffset[3];
class Accelorometer
{
public:
  static void initialise();
  static float* getxyz();

  static int* getxyzraw();

  //returns the current offset on the offset registers
  static int* getoffset();

  static void clearoffsets();

  //this assumes that the adxl is flat on the ground, and completely stationary.
  //this affects the register in the adxl itself, so no load on the arduino itself
  //once the function execution is complete.
  //static int* calibrate(int iterations);
  //currently this is on the arduino itself, idk how efficient that is but i cant for the life of me get it to work on the breakout board registers
  static void calibrate();

};




#endif
