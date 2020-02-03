#version 150

layout(location=0) in vec2 v_pos;
layout(location=1) in vec3 v_col;

out vec3 f_color;

void main() {
	gl_Position = vec4(v_pos, 1.0, 1.0);
	f_color = v_col;
}
