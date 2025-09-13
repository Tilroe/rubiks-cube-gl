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

    // Shaders
    // -------------------------------
    Shader shader = shader_create("../shaders/basic.vert", "../shaders/basic.frag");

    // Set up vertex data
    // -------------------------------
    float vertices[] = {
        // Front face (z = 1)
        -1, -1,  1, // 0
        -1,  1,  1, // 1
         1,  1,  1, // 2
         1, -1,  1, // 3
        // Back face (z = -1)
        -1, -1, -1, // 4
        -1,  1, -1, // 5
         1,  1, -1, // 6
         1, -1, -1, // 7
        // Left face (x = -1)
        -1, -1, -1, // 8
        -1,  1, -1, // 9
        -1,  1,  1, // 10
        -1, -1,  1, // 11
        // Right face (x = 1)
         1, -1,  1, // 12
         1,  1,  1, // 13
         1,  1, -1, // 14
         1, -1, -1, // 15
        // Top face (y = 1)
        -1,  1,  1, // 16
        -1,  1, -1, // 17
         1,  1, -1, // 18
         1,  1,  1, // 19
        // Bottom face (y = -1)
        -1, -1, -1, // 20
        -1, -1,  1, // 21
         1, -1,  1, // 22
         1, -1, -1  // 23
    };

    float texture_info[] = {
        // tex coords   // color
        // Front face (RED)
        0.f, 0.f, RED,
        0.f, 1.f, RED,
        1.f, 1.f, RED,
        1.f, 0.f, RED,
        // Back face (BLUE)
        0.f, 0.f, BLUE,
        0.f, 1.f, BLUE,
        1.f, 1.f, BLUE,
        1.f, 0.f, BLUE,
        // Left face (RED)
        0.f, 0.f, RED,
        0.f, 1.f, RED,
        1.f, 1.f, RED,
        1.f, 0.f, RED,
        // Right face (BLUE)
        0.f, 0.f, BLUE,
        0.f, 1.f, BLUE,
        1.f, 1.f, BLUE,
        1.f, 0.f, BLUE,
        // Top face (RED)
        0.f, 0.f, RED,
        0.f, 1.f, RED,
        1.f, 1.f, RED,
        1.f, 0.f, RED,
        // Bottom face (BLUE)
        0.f, 0.f, BLUE,
        0.f, 1.f, BLUE,
        1.f, 1.f, BLUE,
        1.f, 0.f, BLUE
    };

    unsigned int indices[] = {
        // Front face
        0, 1, 2, 0, 2, 3,
        // Back face
        4, 5, 6, 4, 6, 7,
        // Left face
        8, 9,10, 8,10,11,
        // Right face
       12,13,14,12,14,15,
        // Top face
       16,17,18,16,18,19,
        // Bottom face
       20,21,22,20,22,23
    };

    vec3 positions[] = {
        {20, -1,  2},
        {20,  1, -2}
    };

    unsigned int element_buffer, vertex_buffer, texture_info_buffer, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &texture_info_buffer);
    glGenBuffers(1, &element_buffer);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, texture_info_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_info), texture_info, GL_STATIC_DRAW);

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
    unsigned char *data = stbi_load("../resources/rubiks_texture.jpg", &tex_width, &tex_height, &tex_channels, 0);
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
        get_view_matrix((vec3) { 8, 0, 0 }, (vec3) { 0, 1, 0 }, view_mat);
        get_perspective_matrix(60.f, (float)wwidth / (float)wheight, 0.1f, 100.f, proj_mat);
        set_uniform_mat4f(shader, "view", view_mat);
        set_uniform_mat4f(shader, "projection", proj_mat);


        // Drawing
        glBindVertexArray(VAO);
        for (int p = 0; p < 2; p++) {
            mat4 model_translate, model_rotate, model;

            translation_mat(model_translate, positions[p]);
            quaternion quat = quaternion_create((vec3) { 0, 5, 5 }, (angle += 0.001));
            quaternion_mat(quat, model_rotate);

            mat_mul(model_translate, model_rotate, model);

            set_uniform_mat4f(shader, "model", model);
            glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        }

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