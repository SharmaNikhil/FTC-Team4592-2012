#ifndef globalFunction
#define globalFunction
#include "globalVariables.h"
#include "JoystickDriver.c"
void batteryTest() {
	if(nAvgBatteryLevel < 9000) // NXT Battery should be at 9.0 volts when fully charged
  {
  	PlayTone(100,100);
  }
  wait1Msec(3000);
  if(externalBatteryAvg < 12000) // External Battery Should be at least at 12.0 volts
  {
  	PlayTone(400,100);
  }
}

void liftAssignSafety(float power) {
	#ifdef useSafety
		const int lowestEncoderValue = 0;//7400; //Unknown yet
		if(power < 0) //Trying to go down
		{
			if(!joy2Btn(6)) //Not Overiding
			{
				if(nMotorEncoder[lift] <= lowestEncoderValue)
				{
					power = 0;
				}
			}
		}
	#endif
	//eraseDisplay();
	//nxtDisplayTextLine(4,"Enc Val: %f",nMotorEncoder[lift]);
	motor[lift] = power;
}

#endif
