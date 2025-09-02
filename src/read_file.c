#include "read_file.h"

#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *path)
{
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		perror("read_file");
		return NULL;
	}

	int file_size = 0;
	while (fgetc(file) != EOF) file_size++;
	rewind(file);

	char *str = (char *) malloc(sizeof(char) * (file_size + 1)); // +1 for null terminator
	if (str == NULL) return NULL;
	int c, n = 0;
	while ((c = fgetc(file)) != EOF) 
		*(str + n++) = c;
	str[n] = '\0';

	fclose(file);

	return str;
}
