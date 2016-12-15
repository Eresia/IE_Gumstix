#ifndef VECTOR3_H
#define VECTOR3_H

typedef struct Vector3 Vector3;
struct Vector3{
	double x;
	double y;
	double z;
};

Vector3 addVector3(Vector3 vector1, Vector3 vector2);
Vector3 multiplicateVector3ByDouble(Vector3 vector, double number);
Vector3 divideVector3ByDouble(Vector3 vector, double number);

#endif
