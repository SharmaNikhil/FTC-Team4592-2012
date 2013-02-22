#pragma config(Hubs,  S3, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     irSeek,         sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S2,     light,          sensorLightActive)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     liftSafetyTouch, sensorTouch)
#pragma config(Motor,  mtr_S3_C1_1,     leftDrive,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C1_2,     rightDrive,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S3_C2_1,     lift,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S3_C2_2,     slide,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S3_C4_1,     greenlight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S3_C4_2,     redlight,      tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S3_C3_1,    claw,                 tServoStandard)
#pragma config(Servo,  srvo_S3_C3_2,    clawRelease,          tServoStandard)
#pragma config(Servo,  srvo_S3_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S3_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S3_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S3_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "globalVariables.h"
#include "globalFunctions.c"

void initializeRobot();
void doDriving();
void doGunning();
void releaseClaw();

const int Startup                  = 4;
const int timerDelayForToggles     = 200;
const int driveSpeedToggleBtn      = 2; //Drive Controller
const int slideReverseBtn          = 5;
const int slidePower               = 40;
const int liftUpPower              = 80;
const int liftDownPower            = 80;
const int highestDrivePower        = 100;
const int lowDrivePower            = 50;
const int toggleZeroTolerance      = 10;
const int clawPushPosition         = 160;
const int clawGrabPosition         = 190;
const int clawToggleBtn            = 2; //Gunner Controller
const int clawSlideReleasePosition = 180;

task main()
{
  initializeRobot();

  waitForStart();   // wait for start of tele-op phase



  while (true)
  {
  	getJoystickSettings(joystick);
  	doDriving();
  	doGunning();
  }
}
void initializeRobot()
{
	ClearTimer(T1);
	ClearTimer(T2);
	ClearTimer(T3);
	ClearTimer(T4);
	batteryTest();

  return;
}
void doDriving()


{

	static int drivePower = highestDrivePower;
	if(joy1Btn(driveSpeedToggleBtn) && (time1[T1] > timerDelayForToggles))//if button 2 is pressed change the speed to 50 and if
	{																																			//pressed again it goes back
		if(drivePower == highestDrivePower)//switches the drive power from 50 to 100 or viceversa
			drivePower = lowDrivePower;
		else
			drivePower = highestDrivePower;
		ClearTimer(T1);
	}
	if(abs(joystick.joy1_y1) > toggleZeroTolerance)// depending on the power it moves the robot forward
	{
		motor[leftDrive] = (joystick.joy1_y1/128.0) * drivePower;
	}
	else
	{
		motor[leftDrive] = 0;//if it is not pressed do nothing
	}
	if(abs(joystick.joy1_y2) > toggleZeroTolerance)// same drive system except for right side
	{
		motor[rightDrive] = (joystick.joy1_y2/128.0) * drivePower;
	}
	else
	{
		motor[rightDrive] = 0;
	}
}
void doGunning()
{
	static int slideDirection = 1;
	if(joy2Btn(Startup) && (time1[T2] > timerDelayForToggles))// if button 2 is pressed release the claw
	{

		releaseClaw();
	}
	if(joy2Btn(slideReverseBtn) && (time1[T2] > timerDelayForToggles))//reverse the slide direction
	{
		slideDirection *= -1;
	}
	if( abs(joystick.joy2_x2) > toggleZeroTolerance)
	{
		motor[slide] = (joystick.joy2_x2/128.0) * slidePower * slideDirection;//moves teh slide side to side based on the again it goes back
		//again it goes back
		motor[redlight] = 100;//turns on the redlight
	}
	else
	{
		motor[slide] = 0;//if not press do nothing
		motor[redlight] = 0;
	}
	if(abs(joystick.joy2_y1) > toggleZeroTolerance)//depending on the position on the y axis of the 1st joystick
	{//it moves the slide up
		if(joystick.joy2_y1 > 0)
		{
			liftAssignSafety((joystick.joy2_y1/128.0) * liftUpPower);//goes up
			motor[redlight] = 100;
		}
		else
		{
			liftAssignSafety((joystick.joy2_y1/128.0) * liftDownPower);//goes down
			motor[greenlight] = 100;
		}
	}
	else
	{
		liftAssignSafety(0);//if nothing do nothing
		motor[greenlight] = 0;
		motor[redlight] = 0;
	}
	if(joy2Btn(clawToggleBtn) && (time1[T3] > timerDelayForToggles))// if 2 is pressed change the position of the claw to
	{//push position and if at push postiong put it at grab position
		if(ServoValue[claw] == clawPushPosition)
		{
			servo[claw] = clawGrabPosition;
		}
		else
		{
			servo[claw] = clawPushPosition;
		}
		ClearTimer(T3);
	}
}
void releaseClaw()
{
	servo[clawRelease] = clawSlideReleasePosition;
	wait1Msec(750);
	servo[claw] = clawPushPosition;
}

/*    Drive buttons
* y1 = left drive motor
* y2 = right drive motor
* 2  = toggle speed 50 to 100 %
*     Gunner Controls
* 2 - grab/push rings(starts closed)
* x axis on left joystick- slide motor (left to right)
* y axis on right joystick- lift motor (up and down
* 5 - slide reverse
*/
