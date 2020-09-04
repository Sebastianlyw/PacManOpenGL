#version 440 core
layout (location = 0) in vec4 vert;

out vec2 TexCoords;

uniform mat4 model_matrx;
uniform mat4 projection;

void main()
{
     //gl_Position = vec4(vert.x, vert.y, 0.0, 1.0); 
	gl_Position = projection * model_matrx * vec4(vert.x, vert.y, 0.0, 1.0);
    TexCoords = vert.zw;

}