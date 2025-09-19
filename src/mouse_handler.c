#include "mouse_handler.h"

#include <stdbool.h>
#include <math.h>

#include "window.h"
#include "camera.h"
#include "cube.h"
#include "quaternion.h"
#include "ray.h"

#define M_PI acos(-1.0)

static double xpos, ypos, xvel, yvel;
static int mouse_state[2] = { GLFW_RELEASE, GLFW_RELEASE }; // {lmb, rmb}

static ray calculate_ray();
static void rotate_cube();
static void rotate_face();

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	// button: GLFW_MOUSE_BUTTON_LEFT = 0, GLFW_MOUSE_BUTTON_RIGHT = 1
	mouse_state[button] = action;

	if (mouse_state[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS) {
		rotate_cube();
	}
	else if (mouse_state[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS) {
		ray r = calculate_ray();
	}
}

void cursor_position_callback(GLFWwindow *window, double xpos_new, double ypos_new) {
	xvel = xpos_new - xpos;
	yvel = ypos_new - ypos;
	xpos = xpos_new;
	ypos = ypos_new;
	if (mouse_state[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS) rotate_cube();
}

// https://antongerdelan.net/opengl/raycasting.html

static ray calculate_ray() {
	float x = (2.0f * xpos) / window_width() - 1.0f;
	float y = 1.0f - (2.0f * ypos) / window_height();
	vec4 ray_eye, ray_world, ray_clip = { x, y, -1.0f, 1.0f };
	mat4 proj, view;
	get_perspective_matrix(60.f, (float)window_width() / (float)window_height(), 0.1f, 100.f, proj); mat_inverse(proj);
	get_view_matrix(view); mat_inverse(view);
	
	mat_vec_mul(proj, ray_clip, ray_eye);
	ray_eye[2] = -1.0f; ray_eye[3] = 0.0f;
	mat_vec_mul(view, ray_eye, ray_world);
	vec4_normalize(ray_world);

	ray r;
	vec3_copy(r.origin, *camera_pos());
	vec3_copy(r.direction, ray_world);
	return r;
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
