#include "camera.h"

#include <math.h>

#define M_PI acos(-1.0)

static vec3 pos = { 0, 5, -10 };
static vec3 forward = { 0, 0, 0 };
static vec3 up = { 0, 1, 0 };
static vec3 left = { 0 };

vec3 *camera_pos() {
	return &pos;
}

const vec3 *camera_forward() {
	return &forward;
}

const vec3 *camera_up() {
	return &up;
}

void look_at(const vec3 target) {
	/*
	* OpenGL camera convention is camera at origin with forward down the -z axis
	* This makes x axis on the right, and y axis up
	*
	* So to transform camera coordinates to world coordinates, the mapping is
	* forward -> -z
	* up -> +y
	* left -> -x
	*
	* To get objects into camera space, first translate by the negative of the
	* camera's position, then rotate to the camera's orientation
	*/

	// Foward vector
	vec3_sub(target, pos, forward);
	vec3_normalize(forward);

	// Left vector
	vec3_cross(up, forward, left);
	vec3_normalize(left);

	// Orthogonal up vector (to make orthonormal basis)
	vec3_cross(forward, left, up);
	vec3_normalize(up);
}

void get_view_matrix(mat4 mat) {
	// Transpose to get world->camera
	mat4 rotation = {
		-left[0],		-left[1],		-left[2],		0,
		up[0],			up[1],			up[2],			0,
		-forward[0],	-forward[1],	-forward[2],	0,
		0,				0,				0,				1
	};

	// Translation of camera to origin
	mat4 translation;
	translation_mat((vec3) { -pos[0], -pos[1], -pos[2] }, translation);
	
	// Translate, then rotate
	mat_mul(rotation, translation, mat);
}

// Using https://www.khronos.org/opengl/wiki/GluPerspective_code as reference

void get_perspective_matrix(const float fovy, const float aspect, const float near, const float far, mat4 mat) {
	float t = near * tanf(fovy * (float)M_PI / 360.f);
	float r = t * aspect;
	float b = -t;
	float l = -r;
	float n = near;
	float f = far;

	float r_l = r - l;
	float t_b = t - b;
	float f_n = f - n;

	mat4 pm = {
		2*n / r_l,	0,			(r+l) / r_l,	0,
		0,			2*n/t_b,	(t+b) / t_b,	0,
		0,			0,			-(f+n) / f_n,	-2*f*n/f_n,
		0,			0,			-1,				0
	};
	mat_copy(mat, pm);
}
