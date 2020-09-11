
#include "Ghost.h"
#include "Pacman.h"
#include "gameHelper.h"
#include "..//Utilities/resourceManager.h"
#include "..//Math/AARectangle.h"

namespace
{
	static const uint32_t GHOST_VULNERABE_DURATION = 5000;
	static const uint32_t GHOST_RESPAWN_DURATION = 10000;
}

void Ghost::Init(const char* spritePath, const vec3& initialPos, float movementSpeed)
{
	ResourceManager::LoadShader("./shaders/sprite.vs", "./shaders/sprite.fs", nullptr, "sprite");
	Actor::Init(spritePath, initialPos, movementSpeed);
	mScore = GHOST_SCORE;
	mState = GhostState::GHOST_STATE_INVULNERABLE;
}


void Ghost::Update(uint32_t dt, Pacman& pacman)
{
//	UpdateDirectionFromPacMan(&pacman);
	
	vec2 position = Position();

	Actor::Update(dt);
	
	mCanChangeDirection = position != Position();

	if (IsVulnerable())
	{
		mGhostTimer += dt;
		if (mGhostTimer > GHOST_VULNERABE_DURATION)
		{
			SetGhostState(GhostState::GHOST_STATE_INVULNERABLE);
			mGhostTimer = 0;
		}
	}

	if (IsDead())
	{
		mGhostTimer += dt;
		if (mGhostTimer > GHOST_RESPAWN_DURATION)
		{
			SetGhostState(GhostState::GHOST_STATE_INVULNERABLE);
			mGhostTimer = 0;
		}
	}
}

void Ghost::Draw(uint32_t dt)
{
	ShaderManager shader = ResourceManager::GetShader("sprite");
	
	shader.Use().SetInteger("isVulnerable", mState == GhostState::GHOST_STATE_VULNERABLE);

	shader.SetInteger("isSpeedUp", 0);
	if (!this->IsDead())
	{
		Actor::Draw(dt);
	}
}

void Ghost::SetMovementDirection(PacmanMovement direction)
{
	Actor::SetMovementDirection(direction);
}

void Ghost::Stop()
{
	SetMovementDirection(PacmanMovement::PACMAN_MOVEMENT_NONE);
}

void Ghost::SetToVulnerable()
{
	SetGhostState(GhostState::GHOST_STATE_VULNERABLE);
}

void Ghost::EatenByPacman()
{
	SetGhostState(GhostState::GHOST_STATE_DEAD);
	mSprite->SetPosition(vec3(GHOST_RESPWAN_POSITION, 1));
	mGhostTimer = 0; 
}

void Ghost::ResetToSpwanPosition()
{
	Actor::ResetToSpwanPosition();
	mGhostTimer = 0;
	mCanChangeDirection = true;
	SetGhostState(GhostState::GHOST_STATE_INVULNERABLE);
}


void Ghost::SetGhostState(GhostState state)
{
	mState = state;
	switch (mState)
	{
		// Ghost in normal state is invulnerable.
	case GhostState::GHOST_STATE_INVULNERABLE:
		SetMovementSpeed(GHOST_MOVEMENT_SPEED);
		mGhostTimer = 0;
		break;
	case GhostState::GHOST_STATE_VULNERABLE:
		SetMovementSpeed(GHOST_MOVEMENT_SPEED_SLOW);
		break;
	case GhostState::GHOST_STATE_DEAD:
		SetMovementSpeed(0);
		break;
	}
}