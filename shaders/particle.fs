#version 440 core
out vec4 finalcolor;

in vec4 vertex_color;
in vec2 vertex_uv;

in vec4 particleColor;
uniform sampler2D sprite;

void main()
{
   finalcolor = vertex_color * texture(sprite,vertex_uv) * particleColor;
}
