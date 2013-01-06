#ifndef globalFunction
#define globalFunction
#include "globalVariables.h"

void batteryTest() {
	if(nAvgBatteryLevel < 9000) // NXT Batter should be at 9.0 volts when fully charged
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
	const int liftMiddleEncoderValue = 5000; //Unknown yet
	#ifdef useSafety
		if(SensorValue[liftSafetyTouch] == true)
		{
			if(power > 0) // user wants to raise lift
			{
				if(nMotorEncoder[lift] > liftMiddleEncoderValue)
				{
					power = 0;
				}
			}
			else if(power < 0) // user wants to lower lift
			{
				if(nMotorEncoder[lift] < liftMiddleEncoderValue)
				{
					power = 0;
				}
			}
		}
	#endif
	motor[lift] = power;
}

#endif
