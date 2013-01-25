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
bool atLine();
void stopDrive();

void forward(float distance);

const int grabReleasePositionpeg1 = 28;
const int grabReleasePositionpeg2 = 28;
const int grabReleasePositionpeg3 = 28;
const int grabDownPosition= 90;

task main()
{

  initializeRobot();



	int countline = 0;
  //waitForStart(); // Wait for the beginning of autonomous phase.
  motor[slide] = 20;
	wait1Msec(750);
	motor[slide] = 0;
	ClearTimer(T1);
	SensorValue[irSeek] = 0;
	motor[leftDrive]  = 70;
	motor[rightDrive] = 70;
	servo[leftgrab] = grabDownPosition;

  while(time1[T1] < 9000)//stays in loop while less than 9 seconds
	{
		if(atLine())
		{
			countline++; // if at line check and see if we are at the IR beacon if break out of loop and go to next loop

			if(SensorValue[irSeek] >= 5 && SensorValue[irSeek] <= 6)
			{
				// at Correct Line
				PlayTone(100,100);

				break;

			}
			else if(countline==1)
			{
				motor[rightDrive]= 5;//if it isnt at the beacon and its the first line turn to the right slightly
				motor[leftDrive] = 0;
				wait1Msec(500);
			}
			else if(countline == 2){
				motor[rightDrive] = 2;//if it isn't at the beacon and is at line 2
				motor[leftDrive] = 0;
				wait1Msec(500);
			}
			motor[leftDrive]  = 75;// drive with 75% power
			motor[rightDrive] = 75;

			wait1Msec(500);

		}
	}
	if(time1[T1] < 9000)//while less than 9 seconds and boolean atgood is true
	{
		nMotorEncoder[leftDrive]  = 0;
		nMotorEncoder[rightDrive] = 0;
		if(countline == 1)
		{//if it is at the beacon and it is at the first line

			forward(6); //Distance after line till stop
			stopDrive();

			wait1Msec(500);

			servo[leftgrab] = grabReleasePositionpeg1;//put grabber at position
			wait1Msec(500);
			motor[slide] = -35;
			wait1Msec(900);
			motor[slide] = 0;
			wait1Msec(1000);
	    servo[leftgrab] = grabReleasePositionpeg1 - 15;// move slightly to possible help
	    wait1Msec(1000);
	    motor[leftDrive] = -30;
	    motor[rightDrive] = -30;
	    wait1Msec(1000);
			motor[leftDrive] = 0;
			motor[rightDrive] = 0;
		}
		else if(countline == 2)//
		{//if at line 2 and at the beacon
			forward(7);//Distance after line till stop
			stopDrive();
			wait1Msec(500);

			servo[leftgrab] = grabReleasePositionpeg2;//move to position for the 2nd peg
			wait1Msec(500);
			motor[slide] = -40;//slide over to put the ring on
			wait1Msec(750);
			motor[slide] = 0;
			wait1Msec(1000);
	    servo[leftgrab] = grabReleasePositionpeg2 + 10;//move slightly to help put the ring on
	    wait1Msec(1000);
	    motor[leftDrive] = -50;
	    motor[rightDrive] = -50;
	    wait1Msec(2000);
			motor[leftDrive] = 0;
			motor[rightDrive] = 0;


		}
		else if(countline ==3)
		{//if at line 3 and at beacon
			forward(7.2);//Distance after line till stop
			stopDrive();
			wait1Msec(500);

			servo[leftgrab] = grabReleasePositionpeg3;//put grabber to position for 3rd peg
			wait1Msec(500);
			motor[slide] = -45;//move to slide the ring on
			wait1Msec(750);
			motor[slide] = 0;
			wait1Msec(1000);
	    servo[leftgrab] = grabReleasePositionpeg3 +20;//move slightly to help put ring on
	    wait1Msec(1000);
	    motor[leftDrive] = -50;
	    motor[rightDrive] = -50;
	    wait1Msec(9000);
			forward(7);//move forward to have the ring fall off
		}
	}
	else
		stopDrive();//if not below 9 seconds stop
	nMotorEncoder[leftDrive]  = 0;
	nMotorEncoder[rightDrive] = 0;//clear encoders
  while (true) {}
}

void initializeRobot()
{
  ClearTimer(T1);
	ClearTimer(T2);
	ClearTimer(T3);
	ClearTimer(T4);
	servo[leftgrab] 			 = grabDownPosition;//put grabber at starting position
  while(ServoValue(leftgrab) != grabDownPosition) {}
	batteryTest();

  servo[clawRelease] = clawSlideHoldPosition;// reLEASE SLIDE

  return;
}

bool atLine()
{
	int value = SensorValue[light];
	if(value > 45)//if at line return true else return false
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
