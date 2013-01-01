#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     light,          sensorLightActive)
#pragma config(Sensor, S3,     irSeek,         sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     leftDrive,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     rightDrive,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     slide,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     lift,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    claw,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    clawRelease,          tServoStandard)

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "globalVariables.h"

void initializeRobot();
bool atLine(float value);
void stopDrive();

task main()
{
  initializeRobot();

  //waitForStart(); // Wait for the beginning of autonomous phase.
	ClearTimer(T1);
  while(time1[T1] < 17000)
	{
		if(atLine(SensorValue[light]))
		{
			stopDrive();
			if(SensorValue[irSeek] > 4 && SensorValue[irSeek] < 7)
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
	}
	stopDrive();
  while (true)
  {}
}

void initializeRobot()
{
  ClearTimer(T1);
	ClearTimer(T2);
	ClearTimer(T3);
	ClearTimer(T4);
	/*if(nAvgBatteryLevel < 9000) // NXT Batter should be at 9.0 volts when fully charged
  {
  	PlayTone(100,1000);
  }
  wait1Msec(3000);
  if(externalBatteryAvg < 12000) // External Battery Should be at least at 12.0 volts
  {
  	PlayTone(400,1000);
  }*/
  servo[claw]        = clawStorePosition;
  servo[clawRelease] = clawSlideHoldPosition;
  return;
}

bool atLine(float value)
{
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
