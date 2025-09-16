#include "renderer.h"

#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "shader.h"
#include "window.h"
#include "camera.h"
#include "cube.h"

static Shader shader;
static unsigned int VAO;

static void buffers_init();
static void texture_init();

int renderer_init() {
    // Loading OpenGL function pointers
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // Configure global opengl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Shaders
    shader = shader_create("../shaders/basic.vert", "../shaders/basic.frag");

    // OpenGL generated objects (vao, vbo, ebo, texture)
    buffers_init();
    texture_init();

    return 1;
}

void draw() {
    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    shader_use(shader);

    // Camera & projection
    mat4 view_mat, proj_mat;
    get_view_matrix((vec3) { 0, 0, 0 }, (vec3) { 0, 1, 0 }, view_mat);
    get_perspective_matrix(60.f, (float)window_width() / (float)window_height(), 0.1f, 100.f, proj_mat);
    set_uniform_mat4f(shader, "view", view_mat);
    set_uniform_mat4f(shader, "projection", proj_mat);

    // Drawing
    glBindVertexArray(VAO);

    vec3 cube_position = { 0, 0, 0 };
    mat4 model_translate;

    translation_mat(model_translate, cube_position);
    set_uniform_mat4f(shader, "model", model_translate);

    int element_buffer_len; // TODO: Should I be querying every single time?
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &element_buffer_len);
    glDrawElements(GL_TRIANGLES, element_buffer_len, GL_UNSIGNED_INT, 0);
}

void buffers_init() {
    // Initializing and retrieving actual cube data
    cube_init_data();
    int vertices_size, indices_size, texture_info_size;
    float *vertices = cube_vertex_info(&vertices_size);
    unsigned int *indices = cube_index_info(&indices_size);
    float *texture_info = cube_texture_info(&texture_info_size);

    // Generating OpenGL buffers
    unsigned int element_BO, vertex_BO, texture_info_BO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertex_BO);
    glGenBuffers(1, &texture_info_BO);
    glGenBuffers(1, &element_BO);
    glBindVertexArray(VAO);

    // Binding data
    // 1. Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_BO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    // 2. Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertex_BO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 3. Texture info
    glBindBuffer(GL_ARRAY_BUFFER, texture_info_BO);
    glBufferData(GL_ARRAY_BUFFER, texture_info_size, texture_info, GL_STATIC_DRAW);
    // texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    // colour
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

static void texture_init() {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int tex_width, tex_height, tex_channels;
    unsigned char *data = stbi_load("../resources/rubiks_texture.jpeg", &tex_width, &tex_height, &tex_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);
}