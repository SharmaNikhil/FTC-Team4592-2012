#pragma config(Hubs,  S3, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     irSeek,         sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S2,     light,          sensorLightActive)
#pragma config(Sensor, S4,     liftSafetyTouch, sensorTouch)
#pragma config(Motor,  mtr_S3_C1_1,     leftDrive,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S3_C1_2,     rightDrive,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C2_1,     lift,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S3_C2_2,     slide,         tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S3_C3_1,    claw,                 tServoStandard)
#pragma config(Servo,  srvo_S3_C3_2,    clawRelease,          tServoStandard)
#pragma config(Servo,  srvo_S3_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S3_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C3_5,    leftgrab,             tServoNone)
#pragma config(Servo,  srvo_S3_C3_6,    rightgrab,            tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "globalVariables.h"
#include "globalFunctions.c"

void initializeRobot();

void stopDrive();

void forward(float distance);



task main()
{
  initializeRobot();
	forward(30);
	wait1Msec(750);
	motor[leftDrive] = 30;
	motor[rightDrive] = 0;
  wait1Msec(750);
  forward(10);
  stopDrive();
  while (true) {}
}
void saftey(){

}
void initializeRobot()
{
  ClearTimer(T1);
	ClearTimer(T2);
	ClearTimer(T3);
	ClearTimer(T4);

	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive] = 0;

	batteryTest();



  return;
}



void stopDrive()
{
	motor[leftDrive]  = 0;
	motor[rightDrive] = 0;
}

void forward(float distance) {//forward function to pass in a length in inches and then it goes that for
const float CHANGE = 2;
float totalTraveled = 0;
float encoderTarget = (169.92*distance) - 55.875; //calculate the encoder target
nMotorEncoder[rightDrive] = 0;
nMotorEncoder[leftDrive]  = 0;
float leftEncoder;
float rightEncoder;
float leftPower  = 50;
float rightPower = 50;
while(abs(totalTraveled) < abs(encoderTarget))
{
	leftEncoder  = nMotorEncoder[leftDrive];
	rightEncoder = nMotorEncoder[rightDrive];
	if(leftEncoder > rightEncoder)//changes based on which one has traveled farther
	{
		leftPower  -= CHANGE;
		rightPower += CHANGE;
	}
	else if(leftEncoder < rightEncoder)//same thing
	{
		leftPower  += CHANGE;
		rightPower -= CHANGE;
	}
	motor[leftDrive]  = leftPower;
	motor[rightDrive] = rightPower;
	totalTraveled += (leftEncoder + rightEncoder)/ 2.0;
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive]  = 0;
	ClearTimer(T1);
	while(time1[T1] < 200) {
		leftEncoder  = nMotorEncoder[leftDrive];
		rightEncoder = nMotorEncoder[rightDrive];
		if(!((abs(totalTraveled + (leftEncoder + rightEncoder)/ 2.0)/* average*/)/*adds the distance traveled*/ < abs(encoderTarget)))
		{//while all that is not less than the encoder target
			totalTraveled += (nMotorEncoder[leftDrive] + nMotorEncoder[rightDrive])/ 2.0;// add the aveage of the encoders
			break;//exit
		}
	}
}

motor[leftDrive]  = 0;
motor[rightDrive] = 0;
nMotorEncoder[rightDrive] = 0;
nMotorEncoder[leftDrive]  = 0;
}
