#ifndef SHADER_H
#define SHADER_H

#include "matrix.h"

#define BAD_SHADER 0

typedef unsigned int Shader;

/*
* shader_read: Creates a new OpenGL shader program. A shader program is defined by providing a vertex shader, and a fragment shader
* 
* @param[in] vertex_shader_path: Path to the vertex shader file
* @param[in] fragment_shader_path: Path to the fragment shader file
* 
* @return Resulting shader
*/
Shader shader_create(const char *vertex_shader_path, const char *fragment_shader_path);

/*
* shader_use: Use this shader program in the OpenGL graphics pipeline
* 
* @param[in] shader: shader program to use
*/
void shader_use(Shader shader);

/*
* set_uniform_int: Set a integer uniform value to use in a shader program
*
* @param[in] shader: shader program to configure
* @param[in] name: name of uniform
* @param[in] value: value of uniform
*/
void set_uniform_int(Shader shader, const char *name, int value);

/*
* set_uniform_float: Set a float uniform value to use in a shader program
*
* @param[in] shader: shader program to configure
* @param[in] name: name of uniform
* @param[in] value: value of uniform
*/
void set_uniform_float(Shader shader, const char *name, float value);

/*
* set_uniform_mat4f: Set a Matrix4f uniform value to use in a shader program
*
* @param[in] shader: shader program to configure
* @param[in] name: name of uniform
* @param[in] value: value of uniform
*/
void set_uniform_mat4f(Shader shader, const char *name, mat4 mat);

#endif