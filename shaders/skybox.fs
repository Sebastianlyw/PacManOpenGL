#version 440 core
out vec4 finalcolor;

in vec4 vertex_color;
in vec2 TexCoords;

uniform sampler2D sprite;

void main()
{	
	finalcolor = vec4(1,1,0,0);// vertex_color * texture(sprite,TexCoords);
}
