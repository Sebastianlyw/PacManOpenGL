#version 440 core
out vec4 finalcolor;

in vec4 vertex_color;
in vec2 vertex_uv;

uniform bool isSkyBox;

uniform sampler2D sprite;

void main()
{
    if(isSkyBox)
    {
        finalcolor = vertex_color * texture(sprite,vertex_uv);
    }
    else
    {
        finalcolor = vertex_color * texture(sprite,vertex_uv)*vec4(1,1,1,0.9);
    }
	
}
