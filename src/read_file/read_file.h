#ifndef READ_FILE_H
#define READ_FILE_H

#define STRINGIFY(x) #x

/*
* read_file: Reads an entire file into a string. The string is allocated on the heap, so it must be freed after.
*
* @param[in] path: Path to the file to read
* 
* @return Pointer to string containing file data. Returns NULL if unable to read
*/
char *read_file(const char *path);

#endif