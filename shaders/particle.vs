
#version 440 core

layout(location = 0) in vec2 point;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 model_matrx;
uniform mat4 view;
uniform vec4 pColor;

out vec4 particleColor;
out vec4 vertex_color;
out vec2 vertex_uv;


void main()
{
	gl_Position = projection * view * model_matrx * vec4(point.x, point.y, 0.0, 1.0);
	vertex_color = color;
	vertex_uv = uv;
	particleColor = pColor;

}

