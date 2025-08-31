#ifndef FILE_READER_H
#define FILE_READER_H

#define BAD_SHADER NULL

typedef char* Shader;

/*
* shader_read: Provided a file path, reads into a Shader. Returns
* 
* @param[in] path: Path to the shader
* @return Resulting shader
*/
Shader shader_read(const char* path);

/*
* shader_free: Release memory associated with shader source code
* 
* @param[in] shader: shader to free
*/
void shader_free(Shader shader);


#endif