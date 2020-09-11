#include "Pacman.h"
#include "..//Utilities/Utils.h"
#include "gameHelper.h"
#include "..//Utilities/resourceManager.h"
#include <GLFW/glfw3.h>

namespace 
{
	const uint32_t SPEEDUP_DURATION = 6000; //in milliseconds
}

Pacman::Pacman():mIsDying(false),mScore(0),mSpeedUp(false),mSpeedUpTimer(0), mGhostMultiplier(1)
{
	ResourceManager::LoadShader("./shaders/sprite.vs", "./shaders/sprite.fs", nullptr, "sprite");
}

void Pacman::Init(const char* spritePath, const vec3& initialPos, float movementSpeed)
{
	Actor::Init(spritePath, initialPos, movementSpeed);

	mSprite->AddAnimation("./assets/pacmanWalking.txt", true);

	ResetToFirstAnimation();
	ResetScore();
	mIsDying = false;
	ResetGhostEatenMultiplier();
}

void Pacman::Update(uint32_t dt)
{
	if (mIsDying)
	{
		mSprite->Update(dt);
		mIsDying = !IsFinishedAnimation();
		return;
	}

	if (mSpeedUp)
	{
		mSpeedUpTimer += dt;
		if (mSpeedUpTimer > SPEEDUP_DURATION)
		{
			mSpeedUpTimer = 0;
			mSpeedUp = false;
			SetMovementSpeed(PACMAN_SPEED);
			mSprite->SetSize(PACMAN_SIZE);
		}
	}

	Actor::Update(dt);
}

void Pacman::Reset()
{
	ResetToSpwanPosition();
	ResetScore();
	mIsDying = false;
}

void Pacman::ResetToSpwanPosition()
{
	mIsDying = false;
	Actor::ResetToSpwanPosition();
}

void Pacman::Draw(uint32_t dt)
{
	ShaderManager shader = ResourceManager::GetShader("sprite");


	float timeValue = (float)glfwGetTime();
	float greenValue = (sin(timeValue *10) / 2.0f) + 0.5f;
	shader.Use().SetInteger("isVulnerable", false);
	shader.Use().SetFloat("sinColor", greenValue);
	shader.SetInteger("isSpeedUp", this->mSpeedUp);
	Actor::Draw(dt);
}

void Pacman::SetMovementDirection(PacmanMovement movementDir)
{
	PacmanMovement currentDirection = GetMovementDirection();

	if (movementDir == PacmanMovement::PACMAN_MOVEMENT_LEFT && currentDirection != PacmanMovement::PACMAN_MOVEMENT_LEFT)
	{
		mSprite->transformation.rotation = 0;
		ResetDelta();
	}
	else if (movementDir == PacmanMovement::PACMAN_MOVEMENT_RIGHT && currentDirection != PacmanMovement::PACMAN_MOVEMENT_RIGHT)
	{
		mSprite->transformation.rotation = glm::pi<float>();
		ResetDelta();
	}
	else if (movementDir == PacmanMovement::PACMAN_MOVEMENT_DOWN && currentDirection != PacmanMovement::PACMAN_MOVEMENT_DOWN)
	{
		mSprite->transformation.rotation = glm::pi<float>() / 2 * 3;
		ResetDelta();
	}
	else if (movementDir == PacmanMovement::PACMAN_MOVEMENT_UP && currentDirection != PacmanMovement::PACMAN_MOVEMENT_UP)
	{

		mSprite->transformation.rotation = glm::pi<float>() / 2;
		ResetDelta();
	}

	Actor::SetMovementDirection(movementDir);
}

void Pacman::ResetToFirstAnimation()
{
	Stop();
}

void Pacman::EatenByGhost()
{
	mIsDying = true;
	ResetGhostEatenMultiplier();
}

void Pacman::AteItem(uint32_t value)
{
	AddToScore(value);
}

void Pacman::AteGhost(uint32_t value)
{
	AddToScore(value * mGhostMultiplier);
	mGhostMultiplier *= 2;
}

void Pacman::AddToScore(uint32_t value)
{
	mScore += value;
}

