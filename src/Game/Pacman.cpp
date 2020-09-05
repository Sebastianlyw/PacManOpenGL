#include "Pacman.h"
#include "..//Utilities/Utils.h"
#include "gameHelper.h"
#include "..//Utilities/resourceManager.h"

Pacman::Pacman():mIsDying(false)
{
	ResourceManager::LoadShader("./shaders/sprite.vs", "./shaders/sprite.fs", nullptr, "sprite");
}

void Pacman::Init(const char* spritePath, const vec3& initialPos, uint32_t movementSpeed)
{
	Actor::Init(spritePath, initialPos, movementSpeed);

	mSprite->AddAnimation("./assets/pacmanWalking.txt", true);

	ResetToFirstAnimation();
	ResetScore();
	mIsDying = false;
	ResetGhostEatenMultiplier();
}

void Pacman::Update(double dt)
{
	if (mIsDying)
	{
		mSprite->Update(dt);
		mIsDying = !IsFinishedAnimation();
		return;
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

void Pacman::SetMovementDirection(PacmanMovement movementDir)
{
	PacmanMovement currentDirection = GetMovementDirection();

	if (movementDir == PACMAN_MOVEMENT_LEFT && currentDirection != PACMAN_MOVEMENT_LEFT)
	{
		mSprite->transformation.rotation = 0;
		ResetDelta();
	}
	else if (movementDir == PACMAN_MOVEMENT_RIGHT && currentDirection != PACMAN_MOVEMENT_RIGHT)
	{
		mSprite->transformation.rotation = glm::pi<double>();
		ResetDelta();
	}
	else if (movementDir == PACMAN_MOVEMENT_DOWN && currentDirection != PACMAN_MOVEMENT_DOWN)
	{
		mSprite->transformation.rotation = glm::pi<double>() / 2 * 3;
		ResetDelta();
	}
	else if (movementDir == PACMAN_MOVEMENT_UP && currentDirection != PACMAN_MOVEMENT_UP)
	{

		mSprite->transformation.rotation = glm::pi<double>() / 2;
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

