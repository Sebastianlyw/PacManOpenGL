#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "Sprite.h"
#include "ShaderManager.h"

class PostEffectRender
{
public:
	ShaderManager PostProcessingShader;
	unsigned int Width, Height;
	bool Confuse, Chaos, Shake;
	PostEffectRender(ShaderManager shader, unsigned int width, unsigned int height);
	void BeginRender();
	void EndRender();
	void Render(float time);
private:

	unsigned int FBO; 
	unsigned int RBO; 
	unsigned int VAO;
	unsigned int texture;
	void initRenderData();
};
