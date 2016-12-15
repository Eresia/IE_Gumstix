#include "calculate/calculate.h"

long getTime(struct timeval beginTime, struct timeval actualTime){
	long begin = beginTime.tv_sec * 1000000 + beginTime.tv_usec;
	long actual = actualTime.tv_sec * 1000000 + actualTime.tv_usec;

	return actual - begin;
}

int8_t convertValue(uint8_t value){
	return value;
}

Vector3 calculateAcceleration(){
	Vector3 acceleration;

	acceleration.x = convertValue(getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_X));
	acceleration.y = convertValue(getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_Y));
	acceleration.z = convertValue(getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_Z));

	return acceleration;
}

Vector3 calibrate(long miliseconds){
	struct timeval beginTime, actualTime;
	int i = 0;
	Vector3 average;

	average.x = 0;
	average.y = 0;
	average.z = 0;

	gettimeofday(&beginTime, NULL);

	do{
		average.x += convertValue(getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_X));
		average.y += convertValue(getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_Y));
		average.z += convertValue(getI2cValue(ASK_BUS, ASK_ADDRESS, ASK_Z));
		gettimeofday(&actualTime, NULL);
		i++;
	}while(getTime(beginTime, actualTime) < (miliseconds * 1000));

	average.x /= i;
	average.y /= i;
	average.z /= i;

	return average;
}

Vector3 getCalibrateAcceleration(Vector3 average){
	Vector3 acceleration = calculateAcceleration();

	acceleration.x -= average.x;
	acceleration.y -= average.y;
	acceleration.z -= average.z;

	if((acceleration.x > -ERROR_ACCEL_INTERVAL) && (acceleration.x < ERROR_ACCEL_INTERVAL)){
		acceleration.x = 0;
	}

	if((acceleration.y > -ERROR_ACCEL_INTERVAL) && (acceleration.y < ERROR_ACCEL_INTERVAL)){
		acceleration.y = 0;
	}

	if((acceleration.z > -ERROR_ACCEL_INTERVAL) && (acceleration.z < ERROR_ACCEL_INTERVAL)){
		acceleration.z = 0;
	}

	return acceleration;
}

Vector3 calculateSpeed(Vector3 average, ObjectInformation* informations, double timeInSecond){
	Vector3 result;
	Vector3 accel0 = informations->acceleration;
	Vector3 speed0 = informations->speed;
	Vector3 actualAccel = getCalibrateAcceleration(average);
	Vector3 averageAccel = divideVector3ByDouble(addVector3(actualAccel, accel0), 2.0);

	result = addVector3(speed0, multiplicateVector3ByDouble(averageAccel, timeInSecond));

	informations->acceleration = actualAccel;

	return result;
}

void calculatePosition(Vector3 average, ObjectInformation* informations){
	Vector3 position;
	struct timeval newTime;
	double time;
	Vector3 speed0 = informations->speed;
	Vector3 actualSpeed;
	Vector3 averageSpeed;
	Vector3 p0 = informations->position;

	gettimeofday(&newTime, NULL);
	time = ((double) getTime(informations->lastTime, newTime)) / 1000000.0;

	actualSpeed = calculateSpeed(average, informations, time);

	averageSpeed = divideVector3ByDouble(addVector3(actualSpeed, speed0), 2.0);

	position = addVector3(p0, multiplicateVector3ByDouble(averageSpeed, time));

	informations->lastTime.tv_sec = newTime.tv_sec;
	informations->lastTime.tv_usec = newTime.tv_usec;
	informations->speed = actualSpeed;
	informations->position = position;

}
