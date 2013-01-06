#pragma config(Sensor, S1,     irSeek,         sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S2,     light,          sensorLightActive)
#pragma config(Hubs,   S3, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S4,     liftSafetyTouch, sensorTouch)
#pragma config(Motor,  mtr_S3_C1_1,     leftDrive,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S3_C1_2,     rightDrive,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C2_1,     slide,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C2_2,     lift,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S3_C3_1,    claw,                 tServoStandard)
#pragma config(Servo,  srvo_S3_C3_2,    clawRelease,          tServoStandard)

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "globalVariables.h"
#include "globalFunctions.c"

void initializeRobot();
bool atLine();
void stopDrive();
void forward(float distance);
task main()
{
  initializeRobot();

  //waitForStart(); // Wait for the beginning of autonomous phase.
	/*ClearTimer(T1);
  while(time1[T1] < 17000)
	{
		if(atLine())
		{
			stopDrive();
			if(SensorValueo[irSeek] > 4 && SensorValue[irSeek] < 7)
			{
				// at Correct Line
				break;
			}
			motor[leftDrive]  = 70;
			motor[rightDrive] = 70;
			wait1Msec(750);
		}
		else
		{
			motor[leftDrive]  = 70;
			motor[rightDrive] = 70;
		}
	}*/

	/*motor[leftDrive]  = 70;
	motor[rightDrive] = 70;
	while(!atLine()){}
	nMotorEncoder[leftDrive]  = 0;
	nMotorEncoder[rightDrive] = 0;*/
	forward(15); //Distance after line till stop
	//stopDrive();
  //while (true) {}
}

void initializeRobot()
{
  ClearTimer(T1);
	ClearTimer(T2);
	ClearTimer(T3);
	ClearTimer(T4);
	batteryTest();
  servo[claw]        = clawStorePosition;
  servo[clawRelease] = clawSlideHoldPosition;
  return;
}

bool atLine()
{
	int value = SensorValue[light];
	if(value > 45)
	{
		return true;
	}
	return false;
}

void stopDrive()
{
	motor[leftDrive]  = 0;
	motor[rightDrive] = 0;
}
void forward(float distance) {
const float CHANGE = 2;
float totalTraveled = 0;
float encoderTarget = (169.4*distance) - 15.635;
nMotorEncoder[rightDrive] = 0;
nMotorEncoder[leftDrive]  = 0;
float leftEncoder;
float rightEncoder;
float leftPower = 50;
float rightPower = 50;
while(abs(totalTraveled) < abs(encoderTarget))
{
	leftEncoder  = nMotorEncoder[leftDrive];
	rightEncoder = nMotorEncoder[rightDrive];
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
	motor[leftDrive]  = leftPower;
	motor[rightDrive] = rightPower;
	totalTraveled += (leftEncoder+rightEncoder)/ 2.0;
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive]  = 0;
	ClearTimer(T1);
	while(time1[T1] < 200) {
		if(!(abs(totalTraveled + ((nMotorEncoder[leftDrive] + nMotorEncoder[rightDrive]) / 2.0))) < abs(encoderTarget))
		{
			totalTraveled += (nMotorEncoder[leftDrive]+nMotorEncoder[rightDrive])/ 2.0;
			break;
		}
	}
}
motor[leftDrive]  = 0;
motor[rightDrive] = 0;
nMotorEncoder[rightDrive] = 0;
nMotorEncoder[leftDrive]  = 0;
}
