#ifndef RIGID_C
#define RIGID_C

void forward(float distance); //distance in inches
void run();
void turn(float degrees); //turns 'degrees' degrees right

void rigid() {
	forward(24);
	turn(15);
	foreward(24);
	turn(-15);
}

void turn(float degrees) {
	const float fullturn = 10498;//encoder value for 360 degrees

	float power = 50;
	float turn = degrees*(10498/360);
	while(abs(nMotorEncoder[leftDrive]) < turn && abs(nMotorEncoder[rightDrive]) < turn) {
		if(degrees > 0) {
			motor[leftPower] = -1*power;
			motor[rightDrive] = power;
		}
		else {
			motor[leftDrive] = power;
			motor[rightDrive] = -1*power;
		}
	}
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
}

void forward(float distance) {//forward function to pass in a length in inches and then it goes that for
	const float CHANGE = 2;
	float totalTraveled = 0;
	float encoderTarget = (169.92*distance) - 55.875; //calculate the encoder target
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive]  = 0;
	float leftEncoder;
	float rightEncoder;
	float leftPower  = 50;
	float rightPower = 50;
	while(abs(totalTraveled) < abs(encoderTarget))
	{
		leftEncoder  = nMotorEncoder[leftDrive];
		rightEncoder = nMotorEncoder[rightDrive];
		if(leftEncoder > rightEncoder)//changes based on which one has traveled farther
		{
			leftPower  -= CHANGE;
			rightPower += CHANGE;
		}
		else if(leftEncoder < rightEncoder)//same thing
		{
			leftPower  += CHANGE;
			rightPower -= CHANGE;
		}
		motor[leftDrive]  = leftPower;
		motor[rightDrive] = rightPower;
		totalTraveled += (leftEncoder + rightEncoder)/ 2.0;
		nMotorEncoder[rightDrive] = 0;
		nMotorEncoder[leftDrive]  = 0;
		ClearTimer(T1);
		while(time1[T1] < 200) {
			leftEncoder  = nMotorEncoder[leftDrive];
			rightEncoder = nMotorEncoder[rightDrive];
			if(!((abs(totalTraveled + (leftEncoder + rightEncoder)/ 2.0)/* average*/)/*adds the distance traveled*/ < abs(encoderTarget)))
			{
				//while all that is not less than the encoder target
				totalTraveled += (nMotorEncoder[leftDrive] + nMotorEncoder[rightDrive])/ 2.0;// add the aveage of the encoders
				break;
			}
		}
	}

	motor[leftDrive]  = 0;
	motor[rightDrive] = 0;
	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive]  = 0;
}
#endif