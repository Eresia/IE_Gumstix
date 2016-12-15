#include "calculate/vector3.h"

Vector3 getZeroVector3(){
	Vector3 result;

	result.x = 0;
	result.y = 0;
	result.z = 0;

	return result;
}

void filterVector3(Vector3* vector, double filter){
	if((vector->x > -filter) && (vector->x < filter)){
		vector->x = 0;
	}

	if((vector->y > -filter) && (vector->y < filter)){
		vector->y = 0;
	}

	if((vector->z > -filter) && (vector->z < filter)){
		vector->z = 0;
	}
}

Vector3 addVector3(Vector3 vector1, Vector3 vector2){
	Vector3 result;

	result.x = vector1.x + vector2.x;
	result.y = vector1.y + vector2.y;
	result.z = vector1.z + vector2.z;

	return result;
}

Vector3 multiplicateVector3ByDouble(Vector3 vector, double number){
	Vector3 result;

	result.x = vector.x * number;
	result.y = vector.y * number;
	result.z = vector.z * number;

	return result;
}

Vector3 divideVector3ByDouble(Vector3 vector, double number){
	Vector3 result;

	result.x = vector.x / number;
	result.y = vector.y / number;
	result.z = vector.z / number;

	return result;
}
