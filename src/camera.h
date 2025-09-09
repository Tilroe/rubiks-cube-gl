#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"

void get_view_matrix(const vec3 target, const vec3 up, mat4 mat);

void get_perspective_matrix(const float fovy, const float aspect, const float near, const float far, mat4 mat);

#endif // CAMERA_H
