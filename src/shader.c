#include "shader.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_SHADER_SIZE 1024

Shader shader_read(const char* path) 
{
	Shader shader = (Shader) malloc(sizeof(char) * MAX_SHADER_SIZE);
	if (shader == NULL) return BAD_SHADER;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		perror("Error opening shader\n");
		free(shader);
		return BAD_SHADER;
	}
	
	int c, n = 0;
	while (((c = fgetc(file)) != EOF) && (n < MAX_SHADER_SIZE - 1)) {
		shader[n++] = c;
	}
	fclose(file);

	if (n == MAX_SHADER_SIZE) {
		free(shader);
		return BAD_SHADER;
	}

	shader[n] = '\0';

	return shader;
}
void shader_free(Shader shader)
{
	if (shader == BAD_SHADER) return;
	free(shader);
}

