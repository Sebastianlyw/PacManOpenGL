
#include "Ghost.h"
#include "Pacman.h"
#include "gameHelper.h"
#include "..//Utilities/resourceManager.h"
#include "..//Math/AARectangle.h"

void Ghost::Init(const char* spritePath, const vec3& initialPos, uint32_t movementSpeed)
{
	ResourceManager::LoadShader("./shaders/sprite.vs", "./shaders/sprite.fs", nullptr, "sprite");
	Actor::Init(spritePath, initialPos, movementSpeed);
	mScore = GHOST_SCORE;
	mState = GHOST_STATE_INVULNERABLE;
}


void Ghost::Update(double dt, Pacman& pacman)
{
//	UpdateDirectionFromPacMan(&pacman);
	
	vec2 position = Position();

	Actor::Update(dt);
	
	mCanChangeDirection = position != Position();

	/*if (IsVulnerable())
	{
		mGhostTimer += dt;
		if (mGhostTimer > GHOST_VULNERABE_TIME)
		{
			SetGhostState(GHOST_STATE_INVULNERABLE);
		}
	}*/
}

void Ghost::SetMovementDirection(PacmanMovement direction)
{
	Actor::SetMovementDirection(direction);
}

void Ghost::Stop()
{
	SetMovementDirection(PACMAN_MOVEMENT_NONE);
}

void Ghost::SetToVulnerable()
{
	mState = GHOST_STATE_VULNERABLE;
}

void Ghost::EatenByPacman()
{
	SetGhostState(GHOST_STATE_DEAD);
}

void Ghost::ResetToSpwanPosition()
{
	Actor::ResetToSpwanPosition();
	mGhostTimer = 0;
	mCanChangeDirection = true;
	SetGhostState(GHOST_STATE_INVULNERABLE);
}


void Ghost::SetGhostState(GhostState state)
{
	mState = state;
	switch (mState)
	{
		// Ghost in normal state is invulnerable.
	case GHOST_STATE_INVULNERABLE:
		SetMovementSpeed(GHOST_MOVEMENT_SPEED);
		mGhostTimer = 0;
		break;
	case GHOST_STATE_VULNERABLE:
		SetMovementSpeed(GHOST_MOVEMENT_SPEED_SLOW);
		break;
	case GHOST_STATE_DEAD:
		SetMovementSpeed(0);
		break;
	}
}