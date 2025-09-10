#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "shader.h"
#include "camera.h"
#include "quaternion.h"
#include "matrix.h"
#include "vector.h"


# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600
int wwidth = WINDOW_WIDTH;
int wheight = WINDOW_HEIGHT;

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
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "rubix-cube-gl", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loading OpenGL function pointers
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // Shaders
    // -------------------------------
    Shader shader = shader_create("../shaders/basic.vert", "../shaders/basic.frag");

    // Set up vertex data
    // -------------------------------
    float vertices[] = {
        // position
        -1, -1, -1,
        -1, 1, -1,
        1, -1, -1,
        1, 1, -1,
        -1, -1, 1,
        -1, 1, 1,
        1, -1, 1,
        1, 1, 1,
    };

    vec3 positions[] = {
        {20, -1,  2},
        {20,  1, -2}
    };

    float colours[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        // Front face (z = 0)
        0, 1, 2,
        2, 1, 3,

        // Back face (z = 2)
        4, 6, 5,
        5, 6, 7,

        // Left face (x = 0)
        0, 4, 1,
        1, 4, 5,

        // Right face (x = 2)
        2, 3, 6,
        6, 3, 7,

        // Bottom face (y = 0)
        0, 2, 4,
        4, 2, 6,

        // Top face (y = 2)
        1, 5, 3,
        3, 5, 7
    };

    unsigned int element_buffer, vertex_buffer, colour_buffer, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &colour_buffer);
    glGenBuffers(1, &element_buffer);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    
    float angle = 0.f;
    // Render loop
    // -------------------------------
    while (!glfwWindowShouldClose(window)) {
        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
            glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, 0); // 3 indices per triangle, 2 triangles per face, 6 faces
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