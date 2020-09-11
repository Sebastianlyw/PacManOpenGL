#include "ParticleRender.h"

namespace {
	uint32_t lastUsedParticle = 0;
}

ParticleRender::~ParticleRender()
{
	delete mSprite;
}

ParticleRender::ParticleRender(ShaderManager shader, std::string spriteFilePath, uint32_t amount)
:mShader(shader), mSpriteFilePath(spriteFilePath), mAmount(amount)
{
	this->init();
}

void ParticleRender::Update(uint32_t dt, Pacman& object, uint32_t newParticles, float offsetLen)
{
    for (uint32_t i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
		vec2 offsetVec = GetMovementVector(object.GetMovementDirection());
		offsetVec *= -offsetLen;
        this->respawnParticle(this->mParticles[unusedParticle], object, offsetVec);
    }
    for (uint32_t i = 0; i < this->mAmount; ++i)
    {
        Particle &p = this->mParticles[i];
        p.Life -= dt;
        if (p.Life > 0)
        {
            p.Position -= p.Velocity * static_cast<float>(dt); 
            p.Color.a -= dt/1000.f * 2.5f;
        }
    }
}

void ParticleRender::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->mShader.Use();
	for (Particle particle : this->mParticles)
	{
		if (particle.Life > 0)
		{
			this->mShader.Use().SetMatrix4("projection", Camera::instance().GetPerspectiveProjection());
			this->mShader.SetMatrix4("view", Camera::instance().GetViewMatrix());
			this->mShader.SetVector4f("pColor", particle.Color);
			this->SetPosition(vec3(particle.Position,0));
			this->SetScale(vec2(30));
		
			mShader.SetMatrix4("model_matrx", this->GetTransform());
			this->mSprite->draw(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleRender::init()
{
	this->mSprite = new Sprite(mSpriteFilePath.c_str());
	
	for (uint32_t i = 0; i < this->mAmount; ++i)
		this->mParticles.push_back(Particle());
}

uint32_t ParticleRender::firstUnusedParticle()
{
	for (uint32_t i = lastUsedParticle; i < this->mAmount; ++i) {
		if (this->mParticles[i].Life <= 0) {
			lastUsedParticle = i;
			return i;
		}
	}
	for (uint32_t i = 0; i < lastUsedParticle; ++i) {
		if (this->mParticles[i].Life <= 0) {
			lastUsedParticle = i;
			return i;
		}
	}
	lastUsedParticle = 0;
	return 0;
}

void ParticleRender::respawnParticle(Particle& particle, Pacman& object, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = object.Position() + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1000;  // 1 second.
	particle.Velocity = object.GetVelocity() * 2.0f;
}
