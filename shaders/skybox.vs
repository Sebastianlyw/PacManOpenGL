#version 440 core
layout(location = 0) in vec2 point;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model_matrx;
out vec4 vertex_color;

void main()
{
	gl_Position = projection * model_matrx * vec4(point.x, point.y, 0.0, 1.0);
	vertex_color = color;
    TexCoords = uv;
}  