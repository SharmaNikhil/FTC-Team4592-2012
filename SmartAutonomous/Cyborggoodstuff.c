#ifndef CYBORG_GOOD_STUFF_C
#define CYBORG_GOOD_STUFF_C

#include "drivers/hitechnic-sensormux.h"
#include "drivers/hitechnic-irseeker-v2.h"

const tMUXSensor leftIRDev = msensor_S3_3;
const tMUXSensor rightIRDev = msensor_S3_4;

int right = 0;
int left = 0;

float leftTotal = 0;
float leftAverage = 0;
int rightNumOfReadings = 0;
float rightAverage = 0;
int leftNumOfReadings = 0;
float leftAngle = 0;
float rightAngle = 0;
float rightcurrent = 0;
float leftcurrent = 0;
int rightcount = 0;
int leftcount = 0;
float rightTotal = 0;
const int leftIRZero = 4;
const int leftIR180 = 237;
const int rightIRZero = 6;
const int rightIR180 = 243;

const float dist = 11.5; //inches
const float cornertorack = 99; //inches
const float robottorack = 75;
const float sidepegslines = 10.5;
const float middlepeg = 31;
const float betweenpeg = 20.5;

float x = 0;
float y = 0;
bool left5 = false;
bool right5 = false;

task swivle();
task find();
task updateIRVals();
double tangent();
task leftCalc();
task rightCalc();
void doCyborgVision();


void doCyborgVision(){
	StartTask (updateIRVals);
	StartTask (swivle);
	StartTask (leftCalc);
	StartTask (rightCalc);

}
task updateIRVals() {
    while(true)
    {
        left = HTIRS2readACDir(leftIRDev);
        right = HTIRS2readACDir(rightIRDev);
    }
}
task find(){
    //determine where with IR
	//go based on x and y
}
double Tangent(double a){
	double tan = sin((PI/180.0)*a)/cos((PI/180.0)*a);
	return tan;
}

void PostCalc(){
	x = (-1*dist)*Tangent(leftcurrent);
	y = (-1*dist)*Tangent(rightcurrent);
	//add stuff here
}

task leftCalc() {   //calc when 5 and 6
	while(true)
	{
		if(left == 5)
		{

			leftcount++;
			leftTotal += servo[leftIR];
			leftNumOfReadings++;
			leftAverage = leftTotal / leftNumOfReadings;
			leftAngle = (leftAverage - leftIRZero) / (leftIR180 / 180.0);
			if((leftcount == 4) &&(left5 == false))
			{
				left5 = true;
				leftcurrent = leftAverage;
				leftcount = 0;
				leftNumOfReadings = 0;
				leftTotal = 0;

				PostCalc();
			}
		}
	}
}
task rightCalc() {   //still need to do some editing with this and left
	while(true)
	{
		if(right == 5)
		{
			rightcount++;
			rightTotal += servo[rightIR];
			rightNumOfReadings ++;
			rightAverage = rightTotal /(float) rightNumOfReadings;
			rightAngle = (rightAverage - rightIRZero) / (rightIR180 / 180.0);
			if((rightcount == 4) && (right5 == false)){
				right5 = true;
				rightcurrent = rightAverage;
				rightcount = 0;
				rightTotal = 0;
				rightNumOfReadings = 0;
			}
		}
	}
}
task swivle() {
	bool leftIRGoingLeft = false;
	bool rightIRGoingLeft = false;
	while(true)
	{
		if(servo[leftIR] == leftIR180)
		{
			leftIRGoingLeft = true;
		}
		else if(servo[leftIR] == leftIRZero)
		{
			leftIRGoingLeft = false;
		}
		if(servo[rightIR] == rightIR180)
		{
			rightIRGoingLeft = true;
		}
		else if(servo[rightIR] == rightIRZero)
		{
			rightIRGoingLeft = false;
		}
		if(leftIRGoingLeft)
		{
			servo[leftIR] = servo[rightIR] - 1.0;
		}
		else
		{
			servo[leftIR] = servo[leftIR] + 1.0;
		}
		if(rightIRGoingLeft)
		{
			servo[rightIR] = servo[rightIR] - 1.0;
		}
		else
		{
			servo[rightIR] = servo[rightIR] + 1.0;
		}
		wait1Msec(10);
	}
}

#endif
