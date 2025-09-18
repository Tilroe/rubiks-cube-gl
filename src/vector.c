#include "vector.h"

#include <math.h> // for memcpy
#include <string.h>

static float TOLERANCE = 0.000000001f;
         
void vec3_copy(vec3 dest, const vec3 src) {
    memcpy(dest, src, sizeof(float) * 3);
}

void vec4_copy(vec4 dest, const vec4 src) {
    memcpy(dest, src, sizeof(float) * 4);
}

float vec3_norm(const vec3 vec) {
    return sqrtf((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));
}

float vec4_norm(const vec4 vec) {
    return sqrtf((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]) + (vec[3] * vec[3]));
}

void vec3_normalize(vec3 vec) {
    float magnitude = vec3_norm(vec);
    if (magnitude < TOLERANCE) return;
    vec[0] /= magnitude;
    vec[1] /= magnitude;
    vec[2] /= magnitude;
}

void vec4_normalize(vec4 vec) {
    float magnitude = vec4_norm(vec);
    if (magnitude < TOLERANCE) return;
    vec[0] /= magnitude;
    vec[1] /= magnitude;
    vec[2] /= magnitude;
    vec[3] /= magnitude;
}

float vec3_dot(const vec3 a, const vec3 b) {
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

float vec4_dot(const vec4 a, const vec4 b) {
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) + (a[3] * b[3]);
}

void vec3_cross(const vec3 a, const vec3 b, vec3 c) {
    c[0] = (a[1] * b[2]) - (a[2] * b[1]);
    c[1] = (a[2] * b[0]) - (a[0] * b[2]);
    c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

void vec3_add(const vec3 a, const vec3 b, vec3 c) {
    c[0] = a[0] + b[0];
    c[1] = a[1] + b[1];
    c[2] = a[2] + b[2];
}

void vec3_sub(const vec3 a, const vec3 b, vec3 c) {
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];
}

void vec3_scale(const float scale, vec3 v) {
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}
