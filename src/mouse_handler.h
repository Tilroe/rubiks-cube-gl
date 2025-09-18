#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

#include <GLFW/glfw3.h>

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos_new, double ypos_new);


#endif // !MOUSE_HANDLER_H
