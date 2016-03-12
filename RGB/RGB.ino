#include <Servo.h>
#include <Wire.h>
#include "SparkFunISL29125.h"

// Declare sensor object
SFE_ISL29125 RGB_sensor;

Servo servo;
int servoPin = 3;
int servoOpen = 130;
int servoClose = 90;

Servo ball;
int ballPin = 5;
int ballSpeed = 150;
int ballStop = 1475;

Servo conveyor;
int conveyorPin = 6;
int conveyorSpeed = 150;
int conveyorStop = 1475;

int inputPin = 7;
int outputPin = 4;

int numRunsForColor = 3;
unsigned int cutoff = 300;
int normal = 90;
void setup()
{
	Serial.begin(115200);
	RGB_sensor.init();
	servo.attach(servoPin);
	ball.attach(ballPin);
	conveyor.attach(conveyorPin);
	pinMode(outputPin,INPUT);
	pinMode(inputPin,INPUT);
}

void loop()
{
	if(digitalRead(outputPin) == HIGH && digitalRead(inputPin) == LOW)
	{
		output();
	}
	else if(digitalRead(inputPin) == HIGH && digitalRead(outputPin) == LOW)
	{
		intake();
	}
	else
	{
		halt();
		servo.write(servoOpen);
	}
	//intake();
}
void burst()
{
	startConveyor();
	delay(900);
	halt();
}
void startConveyor() {
	conveyor.writeMicroseconds(conveyorStop - conveyorSpeed);
	ball.writeMicroseconds(ballStop + ballSpeed);
}
void intake()
{
	// Keep running until a ball is seen, and is still seen after stopping motors.
	while (!hasColor() && digitalRead(inputPin) == HIGH && digitalRead(outputPin) == LOW) {
		startConveyor();
		while (!hasColor() && digitalRead(inputPin) == HIGH && digitalRead(outputPin) == LOW) {
			delay(50);
		}
		halt();

	}
	//Check color closes servo to keep
	//servo.write(servoOpen);
	//comment below lines to keep;
	if (false) {}
	else if(checkColor() == "WHITE") {servo.write(servoClose); burst();} 
	//else if(checkColor() == "GREEN") {servo.write(servoClose); burst();}
	//else if(checkColor() == "YELLOW") {servo.write(servoClose); burst();}
	else if(checkColor() == "RED") {servo.write(servoClose); burst();}
	else {servo.write(servoOpen); burst();};                             
}
void output()
{
	//Close servo
	servo.write(servoClose);
	//Reverse conveyor motor
	conveyor.writeMicroseconds(conveyorStop + conveyorSpeed);
	delay(100);
}
void halt()
{
	//Stops motors
	ball.writeMicroseconds(ballStop);
	conveyor.writeMicroseconds(conveyorStop);
	delay(100);
}
bool hasColor() {
	unsigned long red = RGB_sensor.readRed();
	unsigned long green = RGB_sensor.readGreen();
	unsigned long blue = RGB_sensor.readBlue();
	unsigned long light = red + green + blue;
	return light > (cutoff * 3);
}
String checkColor()
{
	// Read sensor values (16 bit integers)
	unsigned long red = 0, green = 0, blue = 0;

	for (int i = 0; i < numRunsForColor; i++) {
		delay(150);
		red += RGB_sensor.readRed();
		green += RGB_sensor.readGreen();
		blue += RGB_sensor.readBlue();
	}

	char line[80];
	unsigned long total = red + green + blue;
	unsigned long average = total / 3;
	unsigned int normalizedRed = (unsigned long)red * 100 / average;
	unsigned int normalizedGreen = (unsigned long)green * 100 / average;
	unsigned int normalizedBlue = (unsigned long)blue * 100 / average;
	sprintf(line, "R: %3d%%, G: %3d%%, B: %3d%%, Average: %d  ", normalizedRed, normalizedGreen, normalizedBlue, average / numRunsForColor);
	Serial.println(line);
	String temp;
	if (red > cutoff || green > cutoff || blue > cutoff) {
		if (normalizedRed > normal-20 && normalizedGreen > normal-20 && normalizedBlue > normal-20) {
			temp = "WHITE";     
			Serial.println("White!");
		}
		else if (normalizedRed > normal && normalizedGreen - 30 <= normal && normalizedBlue <= normal) {
			temp = "RED";      
			Serial.println("Red!"); 
		}
		else if (normalizedRed > normal && normalizedGreen > normal && normalizedBlue <= normal) {
			temp = "YELLOW";      
			Serial.println("Yellow!");
		}
		else if (normalizedRed <= normal && normalizedGreen - 30 > normal && normalizedBlue <= normal) {
			temp = "GREEN";      
			Serial.println("Green!");
		}
		else {
			Serial.println("Some other color!");
		}
	}
	else {
		temp = "BLACK";
		Serial.println("Black!");
	}
	return temp;
}
