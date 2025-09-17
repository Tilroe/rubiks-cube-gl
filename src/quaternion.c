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
	q.x = axis_copy[0] * sine;
	q.y = axis_copy[1] * sine;
	q.z = axis_copy[2] * sine;
	return q;
}

quaternion quaternion_mul(const quaternion a, const quaternion b) {
	vec3 v1 = { a.x, a.y, a.z };
	vec3 v2 = { b.x, b.y, b.z };

	vec3 v3;
	vec3_cross(v1, v2, v3);
	float dot = vec3_dot(v1, v2);

	v3[0] += (a.s * v2[0]) + (b.s * v1[0]);
	v3[1] += (a.s * v2[1]) + (b.s * v1[1]);
	v3[2] += (a.s * v2[2]) + (b.s * v1[2]);
	
	quaternion q;
	q.s = a.s * b.s - dot;
	q.x = v3[0];
	q.y = v3[1];
	q.z = v3[2];

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
