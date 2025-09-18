#include "cube.h"

#include <string.h> // for memcpy

#include "vector.h"
#include "quaternion.h"

/*
3󫢫 Cube (with center cube removed -> 26 total)

Top Layer (1�9):
              Back
        +----+----+----+
        |  1 |  2 |  3 |
        +----+----+----+
 Right  |  4 |  5 |  6 | Left
        +----+----+----+
        |  7 |  8 |  9 |
        +----+----+----+
              Front

Middle Layer (10�17, middle missing):			        +z
              Back
        +----+----+----+								^
        | 10 | 11 | 12 |								|
        +----+----+----+								|
 Right  | 13 |    | 14 | Left				 +x	<------ o			
        +----+----+----+
        | 15 | 16 | 17 |
        +----+----+----+
              Front

Bottom Layer (19�27):
              Back
        +----+----+----+
        | 18 | 19 | 20 |
        +----+----+----+
 Right  | 21 | 22 | 23 | Left
        +----+----+----+
        | 24 | 25 | 26 |
        +----+----+----+
              Front

Original colour orientation:
Top     -   Yellow
Bottom  -   White
Right   -   Red
Left    -   Orange
Front   -   Green
Back    -   Blue
*/

// -------------------------- Colours ----------------------------------

#define GREEN 0.f
#define BLUE 1.f
#define ORANGE 2.f
#define RED 3.f
#define YELLOW 4.f
#define WHITE 5.f
#define BLACK 6.f


// -------------------------- Cube dimensions definitions --------------

#define NUM_CUBES 26
#define FACES_PER_CUBE 6

// Vertex specific
#define VERTEX_PER_FACE 4
#define VERTEX_SIZE 3

// Index specific
#define TRIANGLE_PER_FACE 2
#define INDEX_PER_TRIANGLE 3

// Texture specific
#define TEX_COORD_SIZE 2
#define COLOUR_SIZE 1

// -------------------------- Cube state ------------------------------

static vec3 pos;
static quaternion orientation;


// -------------------------- 1 x 1 x 1 CUBE (Helper) -----------------


static float single_cube_vertices[FACES_PER_CUBE * VERTEX_PER_FACE * VERTEX_SIZE] = {
    // Top face (y + 0.5)
     0.5f,  0.5f, -0.5f,  // front right    1   3
    -0.5f,  0.5f, -0.5f,  // front left     2
    -0.5f,  0.5f,  0.5f,  // back left      3   1
     0.5f,  0.5f,  0.5f,  // back right         2

    // Bottom face (y - 0.5)
     0.5f, -0.5f, -0.5f,  // front right    1   3
    -0.5f, -0.5f, -0.5f,  // front left         2
    -0.5f, -0.5f,  0.5f,  // back left      3   1
     0.5f, -0.5f,  0.5f,  // back right     2

    // Right face (x + 0.5)
     0.5f,  0.5f, -0.5f,  // top front      1   3
     0.5f,  0.5f,  0.5f,  // top back       2
     0.5f, -0.5f,  0.5f,  // bottom back    3   1
     0.5f, -0.5f, -0.5f,  // bottom front       2

    // Left face (x - 0.5)
    -0.5f,  0.5f, -0.5f,  // top front      1   3
    -0.5f,  0.5f,  0.5f,  // top back           2
    -0.5f, -0.5f,  0.5f,  // bottom back    3   1
    -0.5f, -0.5f, -0.5f,  // bottom front   2   
    
    // Front face (z - 0.5)
     0.5f,  0.5f, -0.5f,  // top right      1   3
    -0.5f,  0.5f, -0.5f,  // top left           2
    -0.5f, -0.5f, -0.5f,  // bottom left    3   1
     0.5f, -0.5f, -0.5f,  // bottom right   2

    // Back face (z + 0.5)
     0.5f,  0.5f,  0.5f,  // top right      1   3
    -0.5f,  0.5f,  0.5f,  // top left       2
    -0.5f, -0.5f,  0.5f,  // bottom left    3   1
     0.5f, -0.5f,  0.5f,  // bottom right       2
};

// Defined in such a way as the triangle wind outward
static unsigned int single_cube_indices[FACES_PER_CUBE * TRIANGLE_PER_FACE * INDEX_PER_TRIANGLE] = {
    // Top face
    0, 1, 2,
    2, 3, 0,
    // Bottom face
    4, 7, 6,
    6, 5, 4,
    // Right face
    8, 9, 10,
    10, 11, 8,
    // Left face
    12, 15, 14,
    14, 13, 12,
    // Front face
    16, 19, 18,
    18, 17, 16,
    // Back face
    20, 21, 22,
    22, 23, 20
};

// -------------------------- 3 x 3 x 3 CUBE --------------------------

float vertices[NUM_CUBES * FACES_PER_CUBE * VERTEX_PER_FACE * VERTEX_SIZE] = { 0 };
unsigned int indices[NUM_CUBES * FACES_PER_CUBE * TRIANGLE_PER_FACE * INDEX_PER_TRIANGLE] = { 0 };
float texture_info[NUM_CUBES * FACES_PER_CUBE * VERTEX_PER_FACE * (TEX_COORD_SIZE + COLOUR_SIZE) ] = {
    // ----- TOP LAYER -----
    // Cube 1
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 2
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 3
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 4   
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 5
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 6
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 7
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 8
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 9
    0.f, 0.f, YELLOW,  0.f, 1.f, YELLOW,  1.f, 1.f, YELLOW,  1.f, 0.f, YELLOW,  // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back

    // ----- MIDDLE LAYER -----
    // Cube 10
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 11
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 12
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 13
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 14
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 15
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 16
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 17
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back

    // ----- BOTTOM LAYER -----
    // Cube 18
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 19
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 20
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLUE,    0.f, 1.f, BLUE,    1.f, 1.f, BLUE,    1.f, 0.f, BLUE,    // Back
    // Cube 21
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 22
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 23
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 24
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, RED,     0.f, 1.f, RED,     1.f, 1.f, RED,     1.f, 0.f, RED,     // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 25
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Back
    // Cube 26
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Top
    0.f, 0.f, WHITE,   0.f, 1.f, WHITE,   1.f, 1.f, WHITE,   1.f, 0.f, WHITE,   // Bottom
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK,   // Right
    0.f, 0.f, ORANGE,  0.f, 1.f, ORANGE,  1.f, 1.f, ORANGE,  1.f, 0.f, ORANGE,  // Left
    0.f, 0.f, GREEN,   0.f, 1.f, GREEN,   1.f, 1.f, GREEN,   1.f, 0.f, GREEN,   // Front
    0.f, 0.f, BLACK,   0.f, 1.f, BLACK,   1.f, 1.f, BLACK,   1.f, 0.f, BLACK    // Back
};

static void generate_cube_vertices() {
    int idx = 0;
    int y = 2;
    while (--y >= -1) {
        int z = 2;
        while (--z >= -1) {
            int x = 2;
            while (--x >= -1) {
                if ((x == 0) && (y == 0) && (z == 0)) continue;
                // Copying single cube offsets to next cube, then applying offsets
                memcpy(vertices + idx, single_cube_vertices, sizeof(single_cube_vertices));
                int end_idx = idx + (int) sizeof(single_cube_vertices) / sizeof(float);
                while (idx < end_idx) {
                    vertices[idx++] += x;
                    vertices[idx++] += y;
                    vertices[idx++] += z;
                }
            }
        }
    }
}

static void generate_cube_indices() {
    int idx = 0, cube_idx_offset = 0;
    int y = 2;
    while (--y >= -1) {
        int z = 2;
        while (--z >= -1) {
            int x = 2;
            while (--x >= -1) {
                if ((x == 0) && (y == 0) && (z == 0)) continue;
                // Copying single cube indices of next cube, then adding that cube's idx offset
                memcpy(indices + idx, single_cube_indices, sizeof(single_cube_indices));
                int end_idx = idx + (int) sizeof(single_cube_indices) / sizeof(float);
                while (idx < end_idx) indices[idx++] += cube_idx_offset;
                cube_idx_offset += FACES_PER_CUBE * VERTEX_PER_FACE;
            }
        }
    }
}

vec3 *cube_pos() {
    return &pos;
}

quaternion *cube_orientation() {
    return &orientation;
}

void cube_init_data() {
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;

    orientation = quaternion_create((vec3) { 0, 1, 0 }, 0.f);

    generate_cube_vertices();
    generate_cube_indices();
}

float *cube_vertex_info(int *size) {
    *size = (int)sizeof(vertices);
    return vertices;
}

unsigned int *cube_index_info(int *size) {
    *size = (int)sizeof(indices);
    return indices;
}

float *cube_texture_info(int *size) {
    *size = (int)sizeof(texture_info);
    return texture_info;
}
