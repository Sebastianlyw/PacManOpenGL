#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;

void main()
{    

    FragColor = texture(scene, TexCoords);// vec4(vec3(1.0 - texture(scene, TexCoords)), 1.0);
}