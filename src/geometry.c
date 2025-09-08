#include "geometry.h"

#include <math.h>
#include <string.h>

static float TOLERANCE = 0.00000000f;
         
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



// --------- MATRIX OPERATIONS ---------

void mat_copy(mat4 dest, mat4 src) {
    memcpy(dest, src, sizeof(float) * 16);
}

void ident(mat4 mat) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            mat[COORD_IDX(row, col, 4)] = (row == col) ? 1.0f : 0.0f;
        }
    }
}


void mat_mul(mat4 a, mat4 b, mat4 c) { 
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int partial_sum = 0;
            for (int i = 0; i < 4; i++) {
                partial_sum += a[COORD_IDX(row, i, 4)] * b[COORD_IDX(i, col, 4)];
            }
            c[COORD_IDX(row, col, 4)] = partial_sum;
        }
    }
}

void translate(mat4 mat, vec3 translation) {
    mat4 translation_matrix;
    ident(translation_matrix);
    translation_matrix[COORD_IDX(0, 3, 4)] = translation[0];
    translation_matrix[COORD_IDX(1, 3, 4)] = translation[1];
    translation_matrix[COORD_IDX(2, 3, 4)] = translation[2];
    mat4 temp;
    mat_mul(translation_matrix, mat, temp);
    mat_copy(mat, temp);
}
