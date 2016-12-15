#ifndef ACCEL_VALUE_H
#define ACCEL_VALUE_H

#include <sys/time.h>
#include <stdint.h>

#include "i2c/i2cget.h"
#include "calculate/vector3.h"

#define ERROR_ACCEL_INTERVAL 10
#define ERROR_SPEED_INTERVAL 10

typedef struct ObjectInformation ObjectInformation;
struct ObjectInformation{
	Vector3 acceleration;
	Vector3 speed;
	Vector3 position;
	struct timeval lastTime;
};

Vector3 getAcceleration();
Vector3 calibrate(long miliseconds);

Vector3 getCalibrateAcceleration(Vector3 average);
Vector3 calculateSpeed(Vector3 average, ObjectInformation* informations, double timeInSecond);
void calculatePosition(Vector3 average, ObjectInformation* informations);


#endif
