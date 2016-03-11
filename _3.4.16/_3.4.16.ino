#include <Servo.h>

Servo servo;
int servoPin = 9;
int servoOpen = 124;
int servoClose = 91;

Servo ball;
int ballPin = 10;
int ballSpeed = 100;
int ballStop = 1475;

Servo conveyor;
int conveyorPin = 11;
int conveyorSpeed = 200;
int conveyorStop = 1475;

void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin);
  ball.attach(ballPin);
  conveyor.attach(conveyorPin);
}

void loop()
{
  byte incomingByte = 0;
  byte highNibble = 0;
  byte lowNibble = 0;
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    incomingByte = Serial.read();
    highNibble = incomingByte >> 4;
    lowNibble = incomingByte & 0x0F;
    if (highNibble == lowNibble)
    {
      switch (lowNibble)
      {
        case 1:
          intake();
          break;
        case 2:
          output();
          break;
        default:
          halt();
          break;
      }
    }
  }
  servo.writeMicroseconds(1000);
  delay(2000);
  //servo.writeMicroseconds(2000);
  delay(2000);
}
void intake()
{
  //Check color
  
  //Motors forward
  conveyor.writeMicroseconds(conveyorStop + conveyorSpeed);
  ball.writeMicroseconds(ballStop + ballSpeed);                                  
}
void output()
{
  //Close servo
  servo.write(servoClose);
  //Reverse conveyor motor
  conveyor.writeMicroseconds(conveyorStop - conveyorSpeed);
  delay(100);
}
void halt()
{
  //Stops motors
  ball.writeMicroseconds(ballStop);
  conveyor.writeMicroseconds(conveyorStop);
  //Close servo
  servo.write(servoClose);
  delay(100);
}

