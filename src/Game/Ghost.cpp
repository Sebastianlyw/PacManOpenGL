
#include "Ghost.h"
#include "Pacman.h"
#include "gameHelper.h"
#include "..//Utilities/resourceManager.h"
#include "..//Math/AARectangle.h"

namespace
{
	 const uint32_t GHOST_VULNERABE_DURATION = 6000;
	 const uint32_t GHOST_VULNERABE_ENDING_DURATION = 4000;
	 const uint32_t GHOST_RESPAWN_DURATION = 10000;
}

void Ghost::Init(const char* spritePath, const vec3& initialPos, float movementSpeed)
{
	ResourceManager::LoadShader("./shaders/sprite.vs", "./shaders/sprite.fs", nullptr, "sprite");
	Actor::Init(spritePath, initialPos, movementSpeed);
	mSprite->SetSize(glm::vec2(GHOST_SIZE.x - 1, GHOST_SIZE.y - 1));

	mInitialPosition = initialPos;
	mScore = GHOST_SCORE;
	mState = GhostState::GHOST_STATE_ALIVE;
	ResetToSpwanPosition();
}


void Ghost::Update(uint32_t dt, Pacman& pacman)
{
	
	vec2 position = Position();

	Actor::Update(dt);
	
	mCanChangeDirection = position != Position();

	if (IsVulnerable())
	{
		mGhostTimer += dt;
		if (mState == GhostState::GHOST_STATE_VULNERABLE  &&  mGhostTimer > GHOST_VULNERABE_DURATION)
		{
			SetGhostState(GhostState::GHOST_STATE_VULNERABLE_ENDING);
		}
		if (mState == GhostState::GHOST_STATE_VULNERABLE_ENDING && mGhostTimer > GHOST_VULNERABE_ENDING_DURATION)
		{
			SetGhostState(GhostState::GHOST_STATE_ALIVE);
		}
	}

	/*if (IsDead())
	{
		mGhostTimer += dt;
		if (mGhostTimer > GHOST_RESPAWN_DURATION)
		{
			SetGhostState(GhostState::GHOST_STATE_INVULNERABLE);
			mGhostTimer = 0;
		}
	}*/
}

void Ghost::Draw(uint32_t dt)
{
	ShaderManager shader = ResourceManager::GetShader("sprite");
	
	shader.Use().SetInteger("isVulnerable", mState == GhostState::GHOST_STATE_VULNERABLE);

	shader.SetInteger("isSpeedUp", 0);
	//if (!this->IsDead())
	{
		Actor::Draw(dt);
	}
}

void Ghost::SetMovementDirection(PacmanMovement direction)
{
	Actor::SetMovementDirection(direction);
	if (mState == GhostState::GHOST_STATE_ALIVE)
	{
		//ToDo: Moving Animaiton
	}
	else if (mState == GhostState::GHOST_STATE_DEAD)
	{
		//Todo: dead animation.
	}
}

void Ghost::Stop()
{
	SetMovementDirection(PacmanMovement::PACMAN_MOVEMENT_NONE);
}

void Ghost::SetToVulnerable()
{
	if (mState != GhostState::GHOST_STATE_DEAD && !IsVulnerable())
	{
		SetGhostState(GhostState::GHOST_STATE_VULNERABLE);
	}

}

void Ghost::EatenByPacman()
{
	SetGhostState(GhostState::GHOST_STATE_DEAD);
	//mSprite->SetPosition(vec3(GHOST_RESPWAN_POSITION, 1));
	//mGhostTimer = 0; 
}

void Ghost::ResetToSpwanPosition()
{
	Actor::ResetToSpwanPosition();
	mGhostTimer = 0;
	mCanChangeDirection = true;
	SetGhostState(GhostState::GHOST_STATE_ALIVE);
	mIsReleased = false;

	if (mGhostDelegate)
	{
		mGhostDelegate->GhostWasResetToFirstPosition();
	}

}


void Ghost::ReleaseFromHome()
{
	mIsReleased = true;
	if (mGhostDelegate)
	{
		mGhostDelegate->GhostWasReleasedFromHome();
	}
}

void Ghost::SetGhostState(GhostState state)
{
	if (mGhostDelegate)
	{
		mGhostDelegate->GhostDelegateGhostStateChangeTo(mState, state);
	}

	mState = state;
	switch (mState)
	{
		// Ghost in normal state is invulnerable.
	case GhostState::GHOST_STATE_ALIVE:
		SetMovementDirection(GetMovementDirection()); //???
		SetMovementSpeed(GHOST_MOVEMENT_SPEED);
		break;
	case GhostState::GHOST_STATE_VULNERABLE:
		mGhostTimer = 0;
		SetMovementSpeed(GHOST_MOVEMENT_SPEED_SLOW);
		break;
	case GhostState::GHOST_STATE_VULNERABLE_ENDING:
		mGhostTimer = 0;
		break;
	case GhostState::GHOST_STATE_DEAD:
		SetMovementDirection(GetMovementDirection());
		SetMovementSpeed(GHOST_BACK_TO_PEN_SPEED);
		break;
	}
}