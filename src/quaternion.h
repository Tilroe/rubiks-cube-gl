#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>

#include "vector.h"
#include "matrix.h"

typedef struct {
	float x;
	float y;
	float z;
	float s;
} quaternion;

/*
* quaternion_create: Create a quaternion from an axis and an angle
* 
* @param[in] axis: rotation axis
* @param[in] angle: in degrees
* 
* @return quaternion representation of the rotation
*/
quaternion quaternion_create(const vec3 axis, const float angle);

/*
* quaternion_mat: Generate a rotation matrix from a quaternion
* 
* @param[in] q: quaternion representing rotation. Assuming the quaternion is unit length
* @param[out] mat: sets this matrix to the 4x4 transformation matrix representing the quaternion
*/
void quaternion_mat(const quaternion q, mat4 mat);

#endif // !QUATERNION_H
