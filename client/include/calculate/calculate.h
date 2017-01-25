#ifndef ACCEL_VALUE_H
#define ACCEL_VALUE_H

#include <sys/time.h>
#include <stdint.h>

#include "i2c/i2cget.h"
#include "calculate/vector3.h"

#define IS_GUMSTIX

#define ERROR_ACCEL_INTERVAL 0
#define ERROR_SPEED_INTERVAL 0

#ifndef IS_GUMSTIX
	#define CIRCLE_RAYON 5
#endif

typedef struct ObjectInformation ObjectInformation;
struct ObjectInformation{
	Vector3 acceleration;
	Vector3 speed;
	Vector3 position;
	struct timeval lastTime;
};

#ifndef IS_GUMSTIX
	void initializeSimulator(int nbLoop);
	void incrementSimulatedIterator();
#endif

Vector3 getAcceleration();
Vector3 calibrate(long miliseconds);

Vector3 getCalibrateAcceleration(Vector3 average);
Vector3 calculateSpeed(Vector3 average, ObjectInformation* informations, double timeInSecond);
void calculatePosition(Vector3 average, ObjectInformation* informations);


#endif
