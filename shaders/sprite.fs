#version 440 core
out vec4 finalcolor;

in vec4 vertex_color;
in vec2 vertex_uv;

uniform sampler2D sprite;

void main()
{	
	finalcolor = vertex_color * texture(sprite,vertex_uv);
}
