#version 330

layout(location=0) in vec2 shape_texture;
//layout(location=1) in vec2 _uv;

//out vec2 uv;

void main()
{
	gl_Position = vec4(shape_texture, 1.0, 1.0);
	//uv = vec2(_uv.x, 1.0 - _uv.y);
}
