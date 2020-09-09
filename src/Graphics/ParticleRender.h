#pragma once 

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shaderManager.h"
#include "texture.h"
#include "..//Game/Pacman.h"
#include <string>

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float     Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleRender
{
public:
	~ParticleRender();
	ParticleRender(ShaderManager shader, std::string textureName, uint32_t amount);
	void Update(double dt, Pacman& object, uint32_t newParticles, float offset);
	void Draw();
	inline void SetPosition(vec3 pos) { mSprite->transformation.position = pos; }
	inline void SetScale(vec2 scale) { mSprite->transformation.scale = scale; }
	inline glm::mat4 GetTransform() { return mSprite->transformation.Get(); }
private:
	uint32_t mAmount;
	ShaderManager mShader;
	Sprite* mSprite;
	std::string mSpriteFilePath;
	std::vector<Particle> mParticles;
	void init();
	uint32_t firstUnusedParticle();
	void respawnParticle(Particle& particle, Pacman& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
