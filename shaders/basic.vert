#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord_in;
layout (location = 2) in float colour_in;

out vec2 tex_coord;
out float colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0);
	tex_coord = tex_coord_in;
	colour = colour_in;
}