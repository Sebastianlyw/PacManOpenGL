#version 440 core
out vec4 finalcolor;

in vec4 vertex_color;
in vec2 vertex_uv;

uniform bool isVulnerable;
uniform sampler2D sprite;
uniform float sinColor;
uniform bool isSpeedUp;

void main()
{	
	finalcolor = vertex_color * texture(sprite,vertex_uv);
    if(isVulnerable)
    {
      finalcolor = vertex_color * texture(sprite,vertex_uv) * vec4(0,0.2,1,1);
    }
    if (isSpeedUp)
    {
       finalcolor *= vec4(1,sinColor,1,1);
    }

}
