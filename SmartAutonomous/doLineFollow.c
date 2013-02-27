#ifndef DO_LINE_FOLLOW_C
#define DO_LINE_FOLLOW_C

#include "drivers/hitechnic-sensormux.h"
#include "drivers/lego-light.h"
#include "movement.c"

const tMUXSensor Light1 = msensor_S2_1;
const tMUXSensor Light2 = msensor_S2_2;
const tMUXSensor Light3 = msensor_S2_3;
const tMUXSensor Light4 = msensor_S2_4;
const tMUXSensor Light5 = msensor_S3_1;
const tMUXSensor Light6 = msensor_S3_2;
const float distBetweenLightSens = 1.0 + (7.0/8.0);
const int LINE_TOLERANCE = 80;

int LightSensors[6] = {100,100,100,100,100,100};

bool goingForward = true;
bool correcting = false;
bool hitTarget();
task correctLine();
task MoveForward();
bool OnLine(int a);
bool lineOnRight();
bool lineOnLeft();
bool hitLine(); //Used by doIR and in doLineFollow.c

void doLineFollow()  {
	//int LightSensors[6]; 0 is far left, 4 is far right, 5 is middle
	while(!hitTarget())
	{
		if(OnLine(LightSensors[2])
		{
			if(!goingForward)
			{
				if(correcting)
				{
					StopTask(correctLine);
					correcting = false;
				}
				StartTask(MoveForward);
				goingForward = true;
			}
		}
		else
		{
			if(!correcting)
			{
				if(goingForward)
				{
					StopTask(MoveForward);
					goingForward = false;
				}
				StartTask(correctLine);
				correcting = true;
			}
		}
	}
	StopTask(MoveForward);
	StopTask(correctLine);
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
}
task updateLightSensors() {
	LSsetActive(Light1);
  LSsetActive(Light2);
  LSsetActive(Light3);
  LSsetActive(Light4);
  LSsetActive(Light5);
  LSsetActive(Light6);
	while(true)
	{
		if(safe)
		{
			LightSensors[0] = LSvalNorm(Light1);
			LightSensors[1] = LSvalNorm(Light2);
			LightSensors[2] = LSvalNorm(Light3);
			LightSensors[3] = LSvalNorm(Light4);
			LightSensors[4] = LSvalNorm(Light5);
			LightSensors[5] = LSvalNorm(Light6);
			safe = false;
		}
	}
}
bool OnLine(int a) {
	if(a < LINE_TOLERANCE)
		return true;
	return false;
}
bool hitTarget() {
	if(SensorValue(touch) == 1)
		return true;
	return false;
}
task correctLine() {
	int numDetected;
	for(unsigned int x = 0; x < 5; x++)
	{
		if(OnLine(LightSensors[x]))
		{
			numDetected = x;
			break;
		}
	}
	float distToSide = abs(2 - numDetected) * distBetweenLightSens;
	if(lineOnRight())
	{
		turn(90);
		forward(distToSide);
		turn(-90);
	}
	else if(lineOnLeft())
	{
		turn(-90);
		forward(distToSide);
		turn(90);
	}
	correcting = false;
}
task MoveForward() {
	while(true) {
	    forward(5);
	}
}
bool lineOnRight() {
	 if(OnLine(LightSensors[3]) || OnLine(LightSensors[4]))
		return true;
	return false;
}
bool lineOnLeft() {
	if(OnLine(LightSensors[0]) || OnLine(LightSensors[1]))
		return true;
	return false;
}
bool hitLine() {
	for(unsigned int i = 0; i < 6; i++)
	{
		if(OnLine(LightSensors[i]))
			return true;
	}
	return false;
}
#endif
