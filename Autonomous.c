#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S3,     irseek,         sensorI2CCustom)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     light,          sensorLightActive)
#pragma config(Motor,  mtr_S1_C1_1,     left,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     right,         tmotorTetrix, openLoop, reversed, encoder)

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Autonomous Mode Code Template
//
// This file contains a template for simplified creation of an autonomous program for an Tetrix robot
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "blackMagic.h"

float numofrotations;
float targetencodervalue;
int count = 0;
const int IRDIST = 16;

bool atLine(float value);
void ClockWiseTurn(float degrees);
void forward(float distance);
void CounterClockWiseTurn(float degrees);
task countline();

task main()
{
	forward(7);

	//waitForStart(); // Wait for the beginning of autonomous phase.
/*	while(true)
	{
		if(atLine(SensorValue[light]))
		{
			unsigned int d = HTIRS2readACDir(irseek);
			if(3 < d  && d < 7)
				break;
			wait1Msec(500);
		}
		else
		{
			motor[left]  = 50;
			motor[right] = 50;
		}
	}*/
//clockwise turn or counterclockwise turn in placeRing();
	//PlaceRing();
	while (true)
	{}
}
task countline()
{
	float value = 0;
	while(true)
	{
		value = SensorValue[light];
		if(atLine(value))
		{
			count++;
			wait1Msec(750);
		}

	}
}

bool atLine(float value)
{
	if(value > 45)
	{
		return true;
	}
	return false;
}
void forward(float distance) {
	const float CHANGE = 2;
	int totalTraveled = 0;
	float RotationsNeeded = distance / 7.817;
	float encoderTarget = RotationsNeeded * 1225;
	nMotorEncoder[right] = 0;
	nMotorEncoder[left]  = 0;
	float leftEncoder;
	float rightEncoder;
	float leftPower = 50;
	float rightPower = 100;
	while(abs(totalTraveled) < abs(encoderTarget))
	{
		leftEncoder  = nMotorEncoder[left];
		rightEncoder = nMotorEncoder[right];
		if(leftEncoder > rightEncoder)
		{
			leftPower  -= CHANGE;
			rightPower += CHANGE;
		}
		else if(leftEncoder < rightEncoder)
		{
			leftPower  += CHANGE;
			rightPower -= CHANGE;
		}
		motor[left]  = leftPower;
		motor[right] = rightPower;
		totalTraveled += (leftEncoder+rightEncoder)/2;
		nMotorEncoder[right] = 0;
		nMotorEncoder[left]  = 0;
		wait1Msec(200);
	}
	motor[left] = 0;
	motor[right] = 0;
	nMotorEncoder[right] = 0;
	nMotorEncoder[left]  = 0;
}
void CounterClockWiseTurn(float degrees)
{
	const float PERDEGREE = 30/90.0;
	const float LINKSPERSPROKET = 20;
	numofrotations = (PERDEGREE * degrees )/LINKSPERSPROKET;
	long leftEncoder   = 0;
	long rightEncoder  = 0;
	nMotorEncoder[left]  = 0;
	nMotorEncoder[right] = 0;
	targetencodervalue = numofrotations * 1447;
	motor[left]  = 50;
	motor[right] = -50;
	while((abs(leftEncoder) < targetencodervalue) || (abs(rightEncoder) < targetencodervalue)){
		leftEncoder  = nMotorEncoder[left];
		rightEncoder = nMotorEncoder[right];
		if (abs(rightEncoder) >= targetencodervalue)
			motor[right] = 0;
		if (abs(leftEncoder) >= targetencodervalue)
			motor[left]  = 0;
	}
}
void ClockWiseTurn(float degrees)
{
	const float PERDEGREE = 30/90.0;
	const float LINKSPERSPROKET = 20;
	numofrotations = (PERDEGREE * degrees )/LINKSPERSPROKET;
	long leftEncoder   = 0;
	long rightEncoder  = 0;
	nMotorEncoder[left]  = 0;
	nMotorEncoder[right] = 0;
	targetencodervalue = numofrotations * 1447;
	motor[left] = -50;
	motor[right] = 50;
	while((abs(leftEncoder) < targetencodervalue) || (abs(rightEncoder) < targetencodervalue))
	{//The fuck is this nerd shit?
		leftEncoder  = nMotorEncoder[left];
		rightEncoder = nMotorEncoder[right];
		if (abs(rightEncoder) >= targetencodervalue)
			motor[right] = 0;
		if (abs(leftEncoder) >= targetencodervalue)
			motor[left]  = 0;
	}
}
