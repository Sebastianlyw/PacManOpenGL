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
	uint32_t Width, Height;
	bool Confuse, Chaos, Shake;
	PostEffectRender(ShaderManager shader, uint32_t width, uint32_t height);
	void BeginRender();
	void EndRender();
	void Render(float time);
private:

	uint32_t FBO; 
	uint32_t RBO; 
	uint32_t VAO;
	uint32_t texture;
	void initRenderData();
};
