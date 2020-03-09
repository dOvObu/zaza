#version 330
in vec2 uv;
out vec4 col;

uniform float test;
uniform sampler2D tex;

float circle(vec2 pos, float r)
{
	float l = length(pos);
	return step(l, r);
}

void main()
{
	vec3 txtr = texture(tex, uv).rgb;
	txtr.g *= sin(test);
	col = vec4(txtr, 1.0) * circle(uv, 0.5);
}
