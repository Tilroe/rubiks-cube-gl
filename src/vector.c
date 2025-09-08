#include "vector.h"

#include <math.h>
#include <string.h>

static float TOLERANCE = 0.000000001f;
         
// --------- VECTOR OPERATIONS ---------

float vec3_norm(vec3 vec) {
    return sqrtf((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));
}

float vec4_norm(vec4 vec) {
    return sqrtf((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]) + (vec[3] * vec[3]));
}

void vec3_normalize(vec3 in, vec3 out) {
    float magnitude = vec3_norm(in);
    if (magnitude < TOLERANCE) return;
    out[0] = in[0] / magnitude;
    out[1] = in[1] / magnitude;
    out[2] = in[2] / magnitude;
}

void vec4_normalize(vec4 in, vec4 out) {
    float magnitude = vec3_norm(in);
    if (magnitude < TOLERANCE) return;
    out[0] = in[0] / magnitude;
    out[1] = in[1] / magnitude;
    out[2] = in[2] / magnitude;
    out[3] = in[3] / magnitude;
}

float vec3_dot(vec3 a, vec3 b) {
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

float vec4_dot(vec4 a, vec4 b) {
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) + (a[3] * b[3]);
}

void vec3_cross(vec3 a, vec3 b, vec3 c) {
    c[0] = (a[1] * b[2]) - (a[2] * b[1]);
    c[1] = (a[2] * b[0]) - (a[0] * b[2]);
    c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}
