#version 330 core
out vec4 frag_colour;

in vec2 tex_coord;
in float colour;

uniform sampler2D texture1;

void main() {
	int num_colours = 7;
	float colour_offset = colour / num_colours;
	vec2 actual_tex_coord = vec2(colour_offset + tex_coord.x / num_colours, tex_coord.y);
	frag_colour = texture(texture1, actual_tex_coord);
}