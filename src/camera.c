#include "camera.h"

static vec3 pos; // Camera position

void get_view_matrix(const vec3 target, const vec3 up, mat4 view_mat) {
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
	mat_mul(rotation_mat, translation_mat, view_mat);
}
