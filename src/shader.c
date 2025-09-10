#include "shader.h"

#include <stdlib.h>
#include <stdio.h>

#include <glad/gl.h>

#include "read_file.h"
#include "matrix.h"

static int check_shader_compilation(unsigned int shader_id);
static int check_program_linking(Shader shader);

Shader shader_create(const char *vertex_shader_path, const char *fragment_shader_path) {
	char *vertex_shader_src = read_file(vertex_shader_path);
	char *fragment_shader_src = read_file(fragment_shader_path);
	if ((vertex_shader_src == NULL) || (fragment_shader_src == NULL)) return BAD_SHADER;

	unsigned int vertex_shader, fragment_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);
	check_shader_compilation(vertex_shader);

	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	check_shader_compilation(fragment_shader);

	Shader shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	check_program_linking(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	free(vertex_shader_src);
	free(fragment_shader_src);

	return shader_program;
}

void shader_use(Shader shader) {
	glUseProgram(shader);
}

void set_uniform_int(Shader shader, const char *name, int value) {
	glUniform1i(glGetUniformLocation(shader, name), value);
}

void set_uniform_float(Shader shader, const char *name, float value) {
	glUniform1f(glGetUniformLocation(shader, name), value);
}

void set_uniform_mat4f(Shader shader, const char *name, mat4 mat) {
	glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_TRUE, mat);
}


/*
* check_shader_compilation: Helper function to check if shader compiled successfully in OpenGL
* 
* @param[in] shader_id: OpenGL id for the shader
* 
* @return 0 if successful, non-zero otherwise
*/
static int check_shader_compilation(unsigned int shader_id) {
	int success, log_length;
	char info_log[512];
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader_id, 512, &log_length, info_log);
		printf("Failed to compile shader: %s\n", info_log);
	}
	return success;
}

/*
* check_program_linking: Helper function to check if shader program linked successfully in OpenGL
*
* @param[in] shader: Shader identifier object
* 
* @return 0 if successful, non-zero otherwise
*/
static int check_program_linking(Shader shader) {
	int success, log_length;
	char info_log[512];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, &log_length, info_log);
		printf("Failed to link shader program: %s\n", info_log);
	}
	return success;
}