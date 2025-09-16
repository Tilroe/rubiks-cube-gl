#ifndef CUBE_H
#define CUBE_H

// Sets up cube data (vertex, index, texture)
void cube_init_data();

// Get cube vertex data and size
float *cube_vertex_info(int *size);

// Get cube index data and size
unsigned int *cube_index_info(int *size);

// Get cube texture data and size
float *cube_texture_info(int *size);

#endif // !CUBE_H