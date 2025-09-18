#include "mouse_handler.h"

#include <stdbool.h>
#include <math.h>

#include "camera.h"
#include "cube.h"
#include "quaternion.h"

#include <stdio.h>

#define M_PI acos(-1.0)

static double xpos, ypos, xvel, yvel;
static int mouse_state[2] = { GLFW_RELEASE, GLFW_RELEASE }; // {lmb, rmb}

static void rotate_cube();
static void rotate_face();

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	// button: GLFW_MOUSE_BUTTON_LEFT = 0, GLFW_MOUSE_BUTTON_RIGHT = 1
	mouse_state[button] = action;

	if (mouse_state[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS) {
		rotate_cube();
	}
}

void cursor_position_callback(GLFWwindow *window, double xpos_new, double ypos_new) {
	xvel = xpos_new - xpos;
	yvel = ypos_new - ypos;
	xpos = xpos_new;
	ypos = ypos_new;
	if (mouse_state[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS) rotate_cube();
}

static void rotate_cube() {
	vec3 left, up, rotation_axis;
	vec3_copy(left, *camera_left());
	vec3_copy(up, *camera_up());

	float mouse_movement_magnitude = vec3_norm((vec3) { xvel, yvel, 0 });
	vec3_scale(xvel / mouse_movement_magnitude, up);
	vec3_scale(-yvel / mouse_movement_magnitude, left);
	vec3_add(up, left, rotation_axis);

	float mouse_movement_scaling_factor = 1.0 / 200.0;
	*cube_orientation() = quaternion_mul(
		quaternion_create(rotation_axis, mouse_movement_magnitude * mouse_movement_scaling_factor), 
		*cube_orientation()
	);
}
