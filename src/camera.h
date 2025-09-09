#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"

void get_view_matrix(const vec3 target, const vec3 up, mat4 view_mat);

#endif // CAMERA_H
