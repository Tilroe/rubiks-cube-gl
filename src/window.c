#include "window.h"

#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

// Globals
static GLFWwindow *window;
static int wwidth = 800;
static int wheight = 600;

// Forward declarations
static void window_error_callback(int error_code, const char *description);
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int window_init() {
    glfwSetErrorCallback(window_error_callback);

    if (!glfwInit()) return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(wwidth, wheight, "rubix-cube-gl", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return 1;
}

void window_cleanup() {
    glfwTerminate();
}

int window_should_close() {
    return glfwWindowShouldClose(window);
}

int window_width() {
    return wwidth;
}

int window_height() {
    return wheight;
}

void poll_events() {
    glfwPollEvents();
}

void swap_buffers() {
    glfwSwapBuffers(window);
}

// Forward declaration definitions

void window_error_callback(int error_code, const char *description) {
    printf("GLFW Error %d: %s\n", error_code, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    wwidth = width; wheight = height;
    glViewport(0, 0, wwidth, wheight);
}