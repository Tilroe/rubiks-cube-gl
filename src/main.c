#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

int main() {
    if (!glfwInit()) 
        return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "rubix-cube-gl", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}