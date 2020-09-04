/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "PostEffectRender.h"
#include "..//Game/gameHelper.h"
#include <iostream>
#include <glm/glm.hpp>

PostEffectRender::PostEffectRender(ShaderManager shader, unsigned int width, unsigned int height)
	: PostProcessingShader(shader), Width(width), Height(height), Confuse(false), Chaos(false), Shake(false)
{
	// initialize renderbuffer/framebuffer object
	/*glGenFramebuffers(1, &this->MSFBO);
	glGenFramebuffers(1, &this->FBO);*/
	//glGenRenderbuffers(1, &this->RBO);
	// initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
	//glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height); // allocate storage for render buffer object
	
	/*if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;*/
	// also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
	//glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	glGenFramebuffers(1, &this->FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D,this->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture,0); // attach texture to framebuffer as its color attachment
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;

	glGenRenderbuffers(1, &this->RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	
	// initialize render data and uniforms
	this->initRenderData();
	this->PostProcessingShader.SetInteger("scene", 0, true);
	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right    
	};
	glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (float*)offsets);
	int edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(this->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
	float blur_kernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	glUniform1fv(glGetUniformLocation(this->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostEffectRender::BeginRender()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	 
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);
}
void PostEffectRender::EndRender()
{
	// now resolve multisampled color-buffer into intermediate FBO to store to texture
	/*glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	
	glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostEffectRender::Render(float time)
{
	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	//// clear all relevant buffers
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	//glClear(GL_COLOR_BUFFER_BIT);

	// set uniforms/options
	//this->PostProcessingShader.Use();
	//this->PostProcessingShader.SetFloat("time", time);
	//this->PostProcessingShader.SetInteger("confuse", this->Confuse);
	//this->PostProcessingShader.SetInteger("chaos", this->Chaos);
	//this->PostProcessingShader.SetInteger("shake", this->Shake);
	// render textured quad

	this->PostProcessingShader.Use().SetMatrix4("projection", MainCameraProjection);
	Transform t;
	t.scale = glm::vec2(WINDOWSIZE.x, WINDOWSIZE.y);

//	glViewport(0, 0, WINDOWSIZE.x, WINDOWSIZE.y);
	this->PostProcessingShader.SetMatrix4("model_matrx", t.Get());
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glViewport(0, 0, WINDOWSIZE.x, WINDOWSIZE.y);
	//glBindVertexArray(this->VAO);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);
}

void PostEffectRender::initRenderData()
{
	// configure VAO/VBO
	unsigned int VBO;
	float vertices[] = {
		// pos        // tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}