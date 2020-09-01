#include "PacmanNew.h"
#include "..//Utilities/Utils.h"
#include "gameHelper.h"
#include "..//Utilities/resourceManager.h"

PacmanNew::PacmanNew():mIsDying(false)
{
	ResourceManager::LoadShader("./shaders/sprite.vs", "./shaders/sprite.fs", nullptr, "sprite");
}

void PacmanNew::Init(const char* spritePath, const vec2& initialPos, uint32_t movementSpeed)
{
	Actor::Init(spritePath, initialPos, movementSpeed);

	mSprite->AddAnimation("./assets/pacmanWalking.txt", true);
	mSprite->transformation.scale = PACMAN_SIZE;

	ResetToFirstAnimation();
	ResetScore();
	mIsDying = false;
	ResetGhostEatenMultiplier();
}

void PacmanNew::Update(double dt)
{
	if (mIsDying)
	{
		mSprite->Update(dt);
		mIsDying = !IsFinishedAnimation();
		return;
	}

	Actor::Update(dt);
}

void PacmanNew::SetMovementDirection(PacmanMovement movementDir)
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

void PacmanNew::ResetToFirstAnimation()
{
	//SetAnimation("move_left", true);
	Stop();
}

void PacmanNew::EatenByGhost()
{
	//SetAnimation("death", false);
	mIsDying = true;
	ResetGhostEatenMultiplier();
}

void PacmanNew::ResetScore()
{
	mScore = 0;
}

void PacmanNew::AteItem(uint32_t value)
{
	AddToScore(value);
}

void PacmanNew::AteGhost(uint32_t value)
{
	AddToScore(value * mGhostMultiplier);
	mGhostMultiplier *= 2;
}

void PacmanNew::AddToScore(uint32_t value)
{
	mScore += value;
}

