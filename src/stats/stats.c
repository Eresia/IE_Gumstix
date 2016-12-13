#include <sys/time.h>

#include "stats/stats.h"

long getTime(struct timeval beginTime, struct timeval actualTime){
	long begin = beginTime.tv_sec * 1000000 + beginTime.tv_usec;
	long actual = actualTime.tv_sec * 1000000 + actualTime.tv_usec;

	return actual - begin;
}

AccelValue calculateAcceleration(){
	AccelValue acceleration;

	acceleration.x = getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_X);
	acceleration.y = getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_Y);
	acceleration.z = getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_Z);

	return acceleration;
}

AccelValue calibrate(long miliseconds){
	struct timeval beginTime, actualTime;
	int i = 0;
	AccelValue average;

	average.x = 0;
	average.y = 0;
	average.z = 0;

	gettimeofday(&beginTime, NULL);

	do{
		average.x += getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_X);
		average.y += getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_Y);
		average.z += getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_Z);
		gettimeofday(&actualTime, NULL);
		i++;
	}while(getTime(beginTime, actualTime) < (miliseconds * 1000));

	average.x /= i;
	average.y /= i;
	average.z /= i;

	return average;
}

AccelValue calculateCalibrateAcceleration(AccelValue average){
	AccelValue acceleration = calculateAcceleration();

	acceleration.x -= average.x;
	acceleration.y -= average.y;
	acceleration.z -= average.z;

	return acceleration;
}
