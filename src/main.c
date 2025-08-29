#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600

void error_callback(int error_code, const char* description);

int main() {
    glfwSetErrorCallback(error_callback);

    // GLFW init
    glfwInit();

    // Window creation
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "rubix-cube-gl", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Loading OpenGL function pointers
    int version = gladLoadGL(glfwGetProcAddress);
    printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // Viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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