#ifndef CYBORG_GOOD_STUFF_C
#define CYBORG_GOOD_STUFF_C

#include "drivers/hitechnic-sensormux.h"
#include "drivers/hitechnic-irseeker-v2.h"

typedef struct{
	float leftAngle;
	float rightAngle;
} Angles;
typedef struct{
	float x;
	float y;
} Positions;
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
int passnum;
float sumleftavergae;
float rightTotal = 0;
float lx1;
float lx2;
float ly1;
float ly2;

float rx1;
float rx2;
float ry1;
float ry2;

float leftTotal1 = 0;
float leftAverage1 = 0;
int rightNumOfReadings1 = 0;
float rightAverage1 = 0;
int leftNumOfReadings1 = 0;
float leftAngle1 = 0;
float rightAngle1 = 0;
float rightcurrent1 = 0;
float leftcurrent1 = 0;
int rightcount1 = 0;
int leftcount1 = 0;
float rightTotal1 = 0;

const int leftIRZero = 13.0;
const int leftIR180 = 247.0;
const int rightIRZero = 4.0;
const int rightIR180 = 232.0;

const float dist = 13.5; //inches
const float cornertorack = 99; //inches
const float robottorack = 75;
const float sidepegslines = 10.5;
const float middlepeg = 31;
const float betweenpeg = 20.5;
Angles cur;
Positions acur;

float x = 0;
float y = 0;
bool leftIRGoingLeft = false;
bool rightIRGoingLeft = false;
bool leftin5 = false;
bool rightin5 = false;

task PostCalc();
task swivle();
task updateIRVals();
float getLeftAngles();
float getRightAngles();

void doCyborgVision();
float calcPosX(Angles angles);
float calcPosY(Angles angles);

float calcPosX(Angles angles){
	float a = angles.leftAngle;
	float b = angles.rightAngle;

	if(b > 90){
		float x = ((dist*sinDegrees(a))/sinDegrees(180-a-b))*sinDegrees(90-(180-b))-0.5*dist;
		return x;
	}
	if(a > 90){
		float B = (dist*sinDegrees(b)/sinDegrees(180-a-b));
		float y = B*sinDegrees(180-a);
		return sqrt(B*B-y*y)+0.5*dist;
	}
	if(a == 90){
		return -0.5*dist;
	}
	if(b == 90){
		return 0.5*dist;
	}
	float B = (dist*sinDegrees(b))/sinDegrees(180-a-b);
	float y = ((dist*sinDegrees(a))/sinDegrees(180-a-b))*sinDegrees(b);
	return 0.5*dist-sqrt(B*B-y*y);
}

float calcPosY(Angles angles){
	float a = angles.leftAngle;
	float b = angles.rightAngle;

	if(a > 90){
		float aPrime = 180 - a;
		return (dist*sinDegrees(a)*aPrime)/sinDegrees(180-a-b);
	}
	if(b > 90){
		float bPrime = 180-b;
		float A = ((dist*sinDegrees(a))/sinDegrees(180-a-b));
		return A*sinDegrees(bPrime);
	}

	if(a == 90){
		return (dist*sinDegrees(b))/sinDegrees(180-a-b);
	}
	else
	{
		return (dist*sinDegrees(a))/sinDegrees(180-a-b);

	}

}


void doCyborgVision(){
	StartTask (updateIRVals);
	StartTask (swivle);
	while(true)
	{
		getLeftAngles();
		getRightAngles();
	}
	wait1Msec(9000);
	StartTask (PostCalc);

}
task updateIRVals() {
    while(true)
    {
    	if(!safe)
    	{
        left = HTIRS2readACDir(leftIRDev);
        right = HTIRS2readACDir(rightIRDev);
        safe = true;
      }
    }
}

float Tangent(float a){
	float tan = sin((PI/180.0)*a)/cos((PI/180.0)*a);
	return tan;
}

task PostCalc(){
	while(true)
	{
		x = (-1*dist)* (Tangent(leftcurrent))/(Tangent(leftcurrent))-(Tangent(rightcurrent));
		y = (dist)* Tangent(leftcurrent) * (Tangent(rightcurrent))/(Tangent(rightcurrent)) - (Tangent(leftcurrent));
		//add stuff here
		wait1Msec(1000);
	}
}
float getRightAngles(){
	while(true)
	{
		if((right == 5 && rightIRGoingLeft) &&  (rightin5 == false))
		{
			rightin5 = true;
			ry1 = servo[rightIR];

		}
		if((right > 5 && rightIRGoingLeft) && (rightin5 == true))
		{
				rightin5 = false;
				rx1 = servo[rightIR];
				rightAverage = rx1 + ((ry1 - rx1) / 2.0);

		}
	/*	if((right == 5 && !rightIRGoingLeft) && (rightin5 == false))
		{
			rightin5 = true;
			rx2 = servo[rightIR];

		}
		if((right < 5 && !rightIRGoingLeft) && (rightin5 == true))
		{
				rightin5 = false;
				ry2 = servo[rightIR];
				rightAverage = ry2 + ((ry2 - rx2) / 2);
		}*/
	//	cur.rightAngle = (rightAverage / (rightIR180 - rightIRZero)) * 240.0;
		rightAngle1 = (rightAverage /255.0) * 180.0;
		return rightAngle1;
	}
}

float getLeftAngles(){
	while(true)
	{
	/*	if((left == 5 && (leftIRGoingLeft == false)) && (leftin5 == false))
		{
			leftin5 = true;
			ly1 = servo[leftIR];

		}
		if((left < 5 && (leftIRGoingLeft == false)) && (leftin5 == true))
		{
				leftin5 = false;
				lx1 = servo[leftIR];
				leftNumOfReadings ++;
				leftAverage = lx1 + ((lx1 - ly1) / 2);
		}*/
		if((left == 5 && (leftIRGoingLeft == true)) && (leftin5 == false))
		{
			leftin5 = true;
			lx2 = servo[leftIR];

		}
		if((left > 5 && (leftIRGoingLeft == true)) && (leftin5 == true))
		{
				leftin5 = false;
				ly2 = servo[leftIR];
				leftAverage = ly2 + ((lx2 - ly2) / 2.0);
				//sumleftavergae += leftAverage;
				leftAngle1 = (leftAverage / 255.0) * 180;
		}

		//cur.leftAngle = (leftAverage / (leftIR180 - leftIRZero)) * 180;
		return leftAngle1;
	}
}
task swivle() {
	servo[leftIR] = leftIR180;
	servo[rightIR] = rightIR180;
	wait10Msec(50);
	while(true)
	{
		if(servo[leftIR] == leftIR180)
		{
			leftIRGoingLeft = true;
		}
		else if(servo[leftIR] == leftIRZero)
		{
			leftIRGoingLeft = false;
			passnum++;
		}
		if(servo[rightIR] == rightIR180)
		{
			rightIRGoingLeft = true;
		}
		else if(servo[rightIR] == rightIRZero)
		{
			rightIRGoingLeft = false;
			passnum++;
		}
		if(leftIRGoingLeft)
		{
			servo[leftIR] = servo[leftIR] - 1.0;
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
