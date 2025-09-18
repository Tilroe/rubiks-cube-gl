#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"

vec3* camera_pos();
const vec3* camera_forward();
const vec3* camera_up();
const vec3 *camera_left();

void look_at(const vec3 target);

void get_view_matrix(mat4 mat);

void get_perspective_matrix(const float fovy, const float aspect, const float near, const float far, mat4 mat);

#endif // CAMERA_H
