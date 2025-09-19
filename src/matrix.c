#include "matrix.h"

#include <string.h> // for memcpy

#define ABS(x) ((x > 0) ? x : -x)

static void swap_rows(mat4 mat, const int row1, const int row2) {
    vec4 temp;
    float *r1 = mat + COORD_IDX(row1, 0, 4);
    float *r2 = mat + COORD_IDX(row2, 0, 4);
    memcpy(temp, r1, 4 * sizeof(float));
    memcpy(r1, r2, 4 * sizeof(float));
    memcpy(r2, temp, 4 * sizeof(float));
}

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

void mat_vec_mul(const mat4 a, const vec4 b, vec4 c) {
    for (int row = 0; row < 4; row++) {
        float partial_sum = 0;
        for (int i = 0; i < 4; i++) {
            partial_sum += a[COORD_IDX(row, i, 4)] * b[i];
        }
        c[row] = partial_sum;
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

void mat_inverse(mat4 mat) {
    mat4 augment;
    ident(augment);

    for (int col = 0; col < 4; col++) {
        // 1. Find pivot row
        int pivot_row = col;
        for (int row = col + 1; row < 4; row++) {
            if (ABS(mat[COORD_IDX(row, col, 4)]) > ABS(mat[COORD_IDX(pivot_row, col, 4)])) pivot_row = row;
        }

        // 2. Swap pivot row with row
        swap_rows(mat, col, pivot_row);
        swap_rows(augment, col, pivot_row);

        // 3. Normalize pivot row
        float pivot_val = mat[COORD_IDX(col, col, 4)];
        for (int j = 0; j < 4; j++) {
            mat[COORD_IDX(col, j, 4)] /= pivot_val;
            augment[COORD_IDX(col, j, 4)] /= pivot_val;
        }

        // 4. Set other values in this column to 0
        for (int row = 0; row < 4; row++) {
            if (row != col) {
                float factor = mat[COORD_IDX(row, col, 4)];
                for (int j = 0; j < 4; j++) {
                    mat[COORD_IDX(row, j, 4)] -= factor * mat[COORD_IDX(col, j, 4)];
                    augment[COORD_IDX(row, j, 4)] -= factor * augment[COORD_IDX(col, j, 4)];
                }
            }
        }
    }

    mat_copy(mat, augment);
}