#include "camera.h"

#include <math.h>

#define M_PI acos(-1.0)

static vec3 pos = { 0, 0, 0 }; // Camera position

void get_view_matrix(const vec3 target, const vec3 up, mat4 mat) {
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
	vec3 forward;
	vec3_sub(target, pos, forward);
	vec3_normalize(forward);

	// Right vector
	vec3 left;
	vec3_cross(up, forward, left);
	vec3_normalize(left);

	// Orthogonal up vector (to make orthonormal basis)
	vec3 o_up;
	vec3_copy(o_up, up);
	vec3_cross(forward, left, o_up);
	vec3_normalize(o_up);

	// Transpose to get world->camera
	mat4 rotation_mat = {
		-left[0],		-left[1],		-left[2],		0,
		o_up[0],		o_up[1],		o_up[2],		0,
		-forward[0],	-forward[1],	-forward[2],	0,
		0,				0,				0,				1
	};

	// Translation of camera to origin
	mat4 translation_mat;
	ident(translation_mat);
	translate(translation_mat, (vec3){ -pos[0], -pos[1], -pos[2] });
	
	// Translate, then rotate
	mat_mul(rotation_mat, translation_mat, mat);
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
