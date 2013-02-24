#ifndef DO_LINE_FOLLOW_C
#define DO_LINE_FOLLOW_C

#include "drivers/hitechnic-sensormux.h"
#include "drivers/lego-light.h"

const tMUXSensor Light1 = msensor_S2_1;
const tMUXSensor Light2 = msensor_S2_2;
const tMUXSensor Light3 = msensor_S2_3;
const tMUXSensor Light4 = msensor_S2_4;
const tMUXSensor Light5 = msensor_S3_1;
const tMUXSensor Light6 = msensor_S3_2;

const int LINE_TOLERANCE = 45;

int LightSensors[6] = {0,0,0,0,0,0};

bool goingForward = false;
bool correcting = false;
bool hitTarget();
task correctLine();
task MoveForward();
bool OnLine(int a);
bool lineOnRight();
bool lineOnLeft();
bool hitLine(); //Used by doIR and in doLineFollow.c

void doLineFollow()  {
	//int LightSensors[8] 0 is far left, 6 is far right, 7 is middle
	while(!hitTarget())
	{
		if(OnLine(LightSensors[2]) && OnLine(LightSensors[5]))
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
		if(goingForward)
			StopTask(MoveForward);
		if(correcting)
			StopTask(correctLine);
	}
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
		LightSensors[0] = LSvalNorm(Light1);
		LightSensors[1] = LSvalNorm(Light2);
		LightSensors[2] = LSvalNorm(Light3);
		LightSensors[3] = LSvalNorm(Light4);
		LightSensors[4] = LSvalNorm(Light5);
		LightSensors[5] = LSvalNorm(Light6);
	}
}
bool OnLine(int a) {
	if(a > LINE_TOLERANCE)
		return true;
	return false;
}
bool hitTarget() {
	if(SensorValue[touch] == 1)
		return true;
	return false;
}
task correctLine() {
	typedef enum Direction {
		FORWARD,
		RIGHT_SIDE,
		LEFT_SIDE,
		BACKWARD
	};
	Direction cur_dir = FORWARD; //Direction of IR PEG
	typedef enum Side {
		LEFT,
		RIGHT
	};
	Side lineSide; //Side on wich line was on
	if(lineOnRight())
	{
		lineSide = RIGHT;
	}
	else if(lineOnLeft())
	{
		lineSide = LEFT;
	}
	else
	{
		//Not lined up but also not straight
	}
}
task MoveForward() {
	while(true) {
	    forward(2);
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
