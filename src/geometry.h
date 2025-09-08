#ifndef GEOMETRY_H
#define GEOMETRY_H

#define COORD_IDX(row, col, width) (row * width + col)

typedef float vec3[3];		// {x, y, z}
typedef float vec4[4];		// {x, y, z, w}
typedef float mat4[16];

// Vector operations
float vec3_norm(const vec3 vec);
float vec4_norm(const vec4 vec);

void vec3_normalize(vec3 in, vec3 out);
void vec4_normalize(vec4 in, vec4 out);

float vec3_dot(const vec3 a, const vec3 b);
float vec4_dot(const vec4 a, const vec4 b);

void vec3_cross(vec3 a, vec3 b, vec3 c);

// Matrix Operations

/*
* mat_copy: Copies src values into dest
* 
* @param[in] dest: destination matrix. Copies into this
* @param[in] src: source matrix. Copies from this
*/
void mat_copy(mat4 dest, mat4 src);

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
* @param[in] mat
*/
void translate(mat4 mat, const vec3 translation);
#endif
