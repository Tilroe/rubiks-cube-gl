#include "quaternion.h"

#include <math.h>

#define M_PI acos(-1.0)

// Using https://www.songho.ca/opengl/gl_quaternion.html as a guide

quaternion quaternion_create(const vec3 axis, const float angle) {
	vec3 axis_copy;
	vec3_copy(axis_copy, axis);
	vec3_normalize(axis_copy);
	float rads = angle * (float)M_PI / 180.f;
	float sine = sinf(rads);
	
	quaternion q;
	q.s = cosf(rads);
	q.x = axis[0] * sine;
	q.y = axis[1] * sine;
	q.z = axis[2] * sine;
	return q;
}

void quaternion_mat(const quaternion q, mat4 mat) {
	float x2  = q.x + q.x;
	float y2  = q.y + q.y;
	float z2  = q.z + q.z;
	float xx2 = q.x * x2;
	float xy2 = q.x * y2;
	float xz2 = q.x * z2;
	float yy2 = q.y * y2;
	float yz2 = q.y * z2;
	float zz2 = q.z * z2;
	float sx2 = q.s * x2;
	float sy2 = q.s * y2;
	float sz2 = q.s * z2;

	mat4 rotation_matrix = {
		1 - (yy2 + zz2),	xy2 - sz2,			xz2 + sy2,			0,
		xy2 + sz2,			1 - (xx2 + zz2),	yz2 - sx2,			0,
		xz2 - sy2,			yz2 + sx2,			1 - (xx2 + yy2),	0,
		0,					0,					0,					1
	};

	mat_copy(mat, rotation_matrix);
}
