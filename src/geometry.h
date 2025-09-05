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

// Matrix Operations
void ident(mat4 out);
void translate(mat4 mat, const vec3 translation);
#endif
