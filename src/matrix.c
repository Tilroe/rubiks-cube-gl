#include "matrix.h"

#include <string.h> // for memcpy

void mat_copy(mat4 dest, const mat4 src) {
    memcpy(dest, src, sizeof(float) * 16);
}

void ident(mat4 mat) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            mat[COORD_IDX(row, col, 4)] = (row == col) ? 1.0f : 0.0f;
        }
    }
}


void mat_mul(const mat4 a, const mat4 b, mat4 c) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float partial_sum = 0;
            for (int i = 0; i < 4; i++) {
                partial_sum += a[COORD_IDX(row, i, 4)] * b[COORD_IDX(i, col, 4)];
            }
            c[COORD_IDX(row, col, 4)] = partial_sum;
        }
    }
}

void translation_mat(const vec3 translation, mat4 mat) {
    ident(mat);
    mat[COORD_IDX(0, 3, 4)] = translation[0];
    mat[COORD_IDX(1, 3, 4)] = translation[1];
    mat[COORD_IDX(2, 3, 4)] = translation[2];
}