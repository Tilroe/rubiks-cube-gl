#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <stdlib.h>
#include <stdio.h>

#include "shader.h"
#include "camera.h"
#include "quaternion.h"
#include "matrix.h"
#include "vector.h"
#include "cube.h"

#define GREEN 0.f
#define BLUE 1.f
#define ORANGE 2.f
#define RED 3.f
#define YELLOW 4.f
#define WHITE 5.f
#define BLACK 6.f

int wwidth = 800;
int wheight = 600;

void error_callback(int error_code, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main() {
    glfwSetErrorCallback(error_callback);

    // OpenGL setup
    // -------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation
    GLFWwindow* window = glfwCreateWindow(wwidth, wheight, "rubix-cube-gl", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loading OpenGL function pointers
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Shaders
    // -------------------------------
    Shader shader = shader_create("../shaders/basic.vert", "../shaders/basic.frag");

    // Set up cube data
    // -------------------------------
    setup_cube();
    int vert_size, idx_size, tex_size;
    float *vertices = cube_vertex_info(&vert_size);
    unsigned int *indices = cube_index_info(&idx_size);
    float *texture_info = cube_texture_info(&tex_size);

    unsigned int element_buffer, vertex_buffer, texture_info_buffer, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &texture_info_buffer);
    glGenBuffers(1, &element_buffer);

    glBindVertexArray(VAO);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx_size, indices, GL_STATIC_DRAW);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vert_size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Info
    glBindBuffer(GL_ARRAY_BUFFER, texture_info_buffer);
    glBufferData(GL_ARRAY_BUFFER, tex_size, texture_info, GL_STATIC_DRAW);
        // tex coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
        // colour
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture 
    // -------------------------
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
    
    float angle = 0.f;
    // Render loop
    // -------------------------------
    while (!glfwWindowShouldClose(window)) {
        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Active shader
        shader_use(shader);

        // Camera & projection
        mat4 view_mat, proj_mat;
        get_view_matrix((vec3) { 0, 0, 0 }, (vec3) { 0, 1, 0 }, view_mat);
        get_perspective_matrix(60.f, (float)wwidth / (float)wheight, 0.1f, 100.f, proj_mat);
        set_uniform_mat4f(shader, "view", view_mat);
        set_uniform_mat4f(shader, "projection", proj_mat);

        // Drawing
        glBindVertexArray(VAO);

        vec3 position = { 0, 0, 0 };
        mat4 model_translate, model_rotate, model;
        quaternion rotation = quaternion_create((vec3) { 1, 1, 1 }, (angle += 0.002));

        translation_mat(model_translate, position);
        quaternion_mat(rotation, model_rotate);
        mat_mul(model_translate, model_rotate, model);

        set_uniform_mat4f(shader, "model", model);
        glDrawElements(GL_TRIANGLES, idx_size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        // Swamp buffers and poll I/O
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

void error_callback(int error_code, const char* description) {
    printf("GLFW Error %d: %s\n", error_code, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    wwidth = width; wheight = height;
    glViewport(0, 0, wwidth, wheight);
}