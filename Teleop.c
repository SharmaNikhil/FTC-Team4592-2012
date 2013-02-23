<<<<<<< HEAD
#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     liftSafetyTouch, sensorTouch)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     slide,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     lift,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_3,    clawservo,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    ring,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    leftgrab,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    rightgrab,            tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "globalVariables.h"
#include "globalFunctions.c"

void initializeRobot();
void doDriving();
void doGunning();
void releaseClaw();

const int Releaseboosterbtn        = 3;
const int Startup                  = 7;
const int timerDelayForToggles     = 200;
const int driveSpeedToggleBtn      = 2; //Drive Controller
const int slideReverseBtn          = 5;
const int slidePower               = 40;
const int liftUpPower              = 80;
const int liftDownPower            = 80;
const int highestDrivePower        = 100;
const int lowDrivePower            = 50;
const int toggleZeroTolerance      = 10;
const int clawPushPosition         = 200;
const int clawGrabPosition         = 255;
const int clawToggleBtn            = 8; //Gunner Controller
const int clawSlideReleasePosition = 180;

bool goingToTarget = false;
int targetLiftPos = 0;
//task btnLiftControl();
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
/*task btnLiftControl() { uncomment when encoders are back to normal... george
	int pos = targetLiftPos;
	const int TOLERANCE = 100;
	goingToTarget = true;
	int targetPosition;
	switch (pos)
	{
		case 1:
		targetPosition = 500;
		break;
		case 2:
		targetPosition = 200;
		break;
		case 3:
		targetPosition = 2200;
		break;
		case 4:
		targetPosition = 7900;
		break;
	}
	while(abs(nMotorEncoder[lift] - targetPosition) > TOLERANCE)
	{
		if(nMotorEncoder[lift] > targetPosition)
			liftAssignSafety(-1*liftDownPower);
		else
			liftAssignSafety(liftUpPower);
	}
	goingToTarget = false;
}*/
void initializeRobot()
{
	ClearTimer(T1);
	ClearTimer(T2);
	ClearTimer(T3);
	ClearTimer(T4);
	batteryTest();
	nMotorEncoder[lift] = 0;
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
	}
	else
	{
		motor[slide] = 0;//if not press do nothing
	}
	if(abs(joystick.joy2_y1) > toggleZeroTolerance)//depending on the position on the y axis of the 1st joystick
	{
		//it moves the slide up
		if(goingToTarget)
		{
			//StopTask(btnLiftControl);
			goingToTarget = false;
		}
		if(joystick.joy2_y1 > 0)
		{
			liftAssignSafety((joystick.joy2_y1/128.0) * liftUpPower);//goes up
		}
		else
		{
			liftAssignSafety((joystick.joy2_y1/128.0) * liftDownPower);//goes down
		}
	}
	else
	{
		if(!goingToTarget) liftAssignSafety(0);//if nothing do nothing
	}
	if(joy2Btn(clawToggleBtn) && (time1[T3] > timerDelayForToggles))// if 2 is pressed change the position of the claw to
	{//push position and if at push postiong put it at grab position
		if(ServoValue[clawservo] == clawPushPosition)
		{
			servo[clawservo] = clawGrabPosition;
		}
		else
		{
			servo[clawservo] = clawPushPosition;
		}
		ClearTimer(T3);
	}
	if(joy2Btn(1)){
		//StartTask(btnLiftControl);
		targetLiftPos = 1;
	}
	if(joy2Btn(2)){
		//StartTask(btnLiftControl);
		targetLiftPos = 2;
	}
	if(joy2Btn(3)){
		//StartTask(btnLiftControl);
		targetLiftPos = 3;
	}
	if(joy2Btn(4)){
		//StartTask(btnLiftControl);
		targetLiftPos = 4;
	}
}
void releaseClaw()
{
	servo[clawservo] = 200;
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
=======
#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     liftSafetyTouch, sensorTouch)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     slide,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     lift,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    clawservo,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    leftgrab,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    rightgrab,            tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "globalVariables.h"
#include "globalFunctions.c"

void initializeRobot();
void doDriving();
void doGunning();
void releaseClaw();

const int Releaseboosterbtn        = 3;
const int Startup                  = 7;
const int timerDelayForToggles     = 200;
const int driveSpeedToggleBtn      = 2; //Drive Controller
const int slideReverseBtn          = 5;
const int slidePower               = 40;
const int liftUpPower              = 80;
const int liftDownPower            = 80;
const int highestDrivePower        = 100;
const int lowDrivePower            = 50;
const int toggleZeroTolerance      = 10;
const int clawPushPosition         = 200;
const int clawGrabPosition         = 255;
const int clawToggleBtn            = 8; //Gunner Controller
const int clawSlideReleasePosition = 180;

bool goingToTarget = false;
int targetLiftPos = 0;
//task btnLiftControl();
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
/*task btnLiftControl() { uncomment when encoders are back to normal... george
	int pos = targetLiftPos;
	const int TOLERANCE = 100;
	goingToTarget = true;
	int targetPosition;
	switch (pos)
	{
		case 1:
		targetPosition = 500;
		break;
		case 2:
		targetPosition = 200;
		break;
		case 3:
		targetPosition = 2200;
		break;
		case 4:
		targetPosition = 7900;
		break;
	}
	while(abs(nMotorEncoder[lift] - targetPosition) > TOLERANCE)
	{
		if(nMotorEncoder[lift] > targetPosition)
			liftAssignSafety(-1*liftDownPower);
		else
			liftAssignSafety(liftUpPower);
	}
	goingToTarget = false;
}*/
void initializeRobot()
{
	ClearTimer(T1);
	ClearTimer(T2);
	ClearTimer(T3);
	ClearTimer(T4);
	batteryTest();
	nMotorEncoder[lift] = 0;
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
		motor[slide] = (joystick.joy2_x2/128.0) * slidePower * slideDirection;//moves the slide side to side based on the again it goes back
		//again it goes back
	}
	else
	{
		motor[slide] = 0;//if not press do nothing
	}
	if(abs(joystick.joy2_y1) > toggleZeroTolerance)//depending on the position on the y axis of the 1st joystick
	{
		//it moves the slide up
		if(goingToTarget)
		{
			//StopTask(btnLiftControl);
			goingToTarget = false;
		}
		if(joystick.joy2_y1 > 0)
		{
			liftAssignSafety((joystick.joy2_y1/128.0) * liftUpPower);//goes up
		}
		else
		{
			liftAssignSafety((joystick.joy2_y1/128.0) * liftDownPower);//goes down
		}
	}
	else
	{
		if(!goingToTarget) liftAssignSafety(0);//if nothing do nothing
	}
	if(joy2Btn(clawToggleBtn) && (time1[T3] > timerDelayForToggles))// if 2 is pressed change the position of the claw to
	{//push position and if at push postiong put it at grab position
		if(ServoValue[clawservo] == clawPushPosition)
		{
			servo[clawservo] = clawGrabPosition;
		}
		else
		{
			servo[clawservo] = clawPushPosition;
		}
		ClearTimer(T3);
	}
	if(joy2Btn(1)){
		//StartTask(btnLiftControl);
		targetLiftPos = 1;
	}
	if(joy2Btn(2)){
		//StartTask(btnLiftControl);
		targetLiftPos = 2;
	}
	if(joy2Btn(3)){
		//StartTask(btnLiftControl);
		targetLiftPos = 3;
	}
	if(joy2Btn(4)){
		//StartTask(btnLiftControl);
		targetLiftPos = 4;
	}
}
void releaseClaw()
{
	servo[clawservo] = 200;
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
>>>>>>> 9dbc7c6b848996938ef4284aeeb48c3b2d8a203a
