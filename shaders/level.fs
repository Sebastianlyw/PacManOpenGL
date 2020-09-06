#version 440 core
out vec4 finalcolor;

in vec4 vertex_color;
in vec2 vertex_uv;

uniform bool isSkyBox;

uniform float deltaTime;

uniform sampler2D sprite;
uniform float ourColor; // we set this variable in the OpenGL code.

uniform float sinColor;
uniform bool isSuperPacman;

void main()
{
    if(isSkyBox)
    {
     vec2 uv = vertex_uv;
     uv.y += deltaTime;
     finalcolor = vertex_color * texture(sprite,uv) * vec4(1,1,1,0.9);
        
     //   uniform float dt;
    }
    else
    {
     //finalcolor = vec4(0,ourColor,0,1);
        finalcolor = vertex_color * texture(sprite,vertex_uv)*vec4(1,1,1,0.9);
        if(isSuperPacman)
        {
            finalcolor *= vec4(1,sinColor,1,1);
        }
    }

   
}
