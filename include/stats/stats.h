#ifndef ACCEL_VALUE_H
#define ACCEL_VALUE_H

#include "i2c/i2cget.h"

typedef struct AccelValue AccelValue;
struct AccelValue{
	int x;
	int y;
	int z;
};

#endif

AccelValue calculateAcceleration();
AccelValue calculateAverage();
