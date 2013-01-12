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
void doDriving();
void doGunning();
void releaseClaw();


const int timerDelayForToggles     = 750;
const int driveSpeedToggleBtn      = 2; //Drive Controller
const int slideReverseBtn          = 5;
const int slidePower               = 20;
const int liftUpPower              = 70;
const int liftDownPower            = 60;
const int highestDrivePower        = 100;
const int lowDrivePower            = 25;
const int toggleZeroTolerance      = 10;
const int clawPushPosition         = 190;
const int clawGrabPosition         = 225;
const int clawToggleBtn            = 2; //Gunner Controller
const int clawSlideReleasePosition = 180;

task main()
{
  initializeRobot();

  waitForStart();   // wait for start of tele-op phase
	liftAssignSafety(liftUpPower);
	wait1Msec(1000);
	releaseClaw();
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
  servo[claw]        = clawStorePosition;
  servo[clawRelease] = clawSlideHoldPosition;
  return;
}
void doDriving()
{
	static int drivePower = highestDrivePower;
	if(joy1Btn(driveSpeedToggleBtn) && (time1[T1] > timerDelayForToggles))
	{
		if(drivePower == highestDrivePower)
			drivePower = lowDrivePower;
		else
			drivePower = highestDrivePower;
		ClearTimer(T1);
	}
	if(abs(joystick.joy1_y1) > toggleZeroTolerance)
	{
		motor[leftDrive] = (joystick.joy1_y1/128.0) * drivePower;
	}
	else
	{
		motor[leftDrive] = 0;
	}
	if(abs(joystick.joy1_y2) > toggleZeroTolerance)
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
	if(joy2Btn(slideReverseBtn) && (time1[T2] > timerDelayForToggles))
	{
		slideDirection *= -1;
	}
	if( abs(joystick.joy2_x2) > toggleZeroTolerance)
	{
		motor[slide] = (joystick.joy2_x2/128.0) * slidePower * slideDirection;
	}
	else
	{
		motor[slide] = 0;
	}
	if( abs(joystick.joy2_y1) > toggleZeroTolerance)
	{
		if(joystick.joy2_y1 > 0)
		{
			liftAssignSafety((joystick.joy2_y1/128.0) * liftUpPower);
		}
		else
		{
			liftAssignSafety((joystick.joy2_y1/128.0) * liftDownPower);
		}
	}
	else
	{
		liftAssignSafety(0);
	}
	if(joy2Btn(clawToggleBtn) && (time1[T3] > timerDelayForToggles))
	{
		if(ServoValue[claw] == clawPushPosition)
		{
			servo[claw] = clawGrabPosition;
		}
		else
		{
			servo[claw] = clawPushPosition;
		}
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
