#ifndef MATRIX_H
#define MATRIX_H

#define COORD_IDX(row, col, width) (row * width + col)

#include "vector.h"

typedef float mat4[16];

void mat_copy(mat4 dest, const mat4 src);

/*
* Turn a matrix into an identity matrix
*
* @param[in] mat: matrix to turn into the identity matrix
*/
void ident(mat4 mat);

/*
* mat_mul: Multiplies a by b (a on the left, b on the right). Stores the result in matrix c
*
* @param[in] a: left matrix in multiplication
* @param[in] b: right matrix in multiplication
* @param[out] c: result of multiplication
*/
void mat_mul(mat4 a, mat4 b, mat4 c);

/*
* translate: Applies a translation to a given transformation matrix
*
* @param[in] mat: transformation matrix to modify
* @param[in] translation: how much to translate by 
*/
void translate(mat4 mat, const vec3 translation);


#endif // !MATRIX_H
