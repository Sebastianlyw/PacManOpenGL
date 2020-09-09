#pragma once

#include "Ghost.h"
#include "gameHelper.h"
#include <random>

class PacmanLevel;

enum class GhostAIState :unsigned char
{
	GHOST_STATE_START		= 0x01,
	GHOST_STATE_SCATTER		= 0x02,
	GHOST_STATE_CHASE		= 0x04,
	GHOST_STATE_AT_HOME		= 0x08,
	GHOST_STATE_EXIT_HOME	= 0x10,
	GHOST_STATE_GO_HOME		= 0x20
};

class GhostAI
{

public :
	GhostAI():mGhost(nullptr), mName(GhostName::RED), mState(GhostAIState::GHOST_STATE_CHASE), mLastState(GhostAIState::GHOST_STATE_CHASE)
	, mScatterTarget(vec2(0.f)), mTarget(vec2(0.f)),mTimer(0), mLookAheadDistance(0){}

	void Init(Ghost& ghost, uint32_t lookAheadDistance, const vec2& scatterTarget, GhostName name);

	PacmanMovement Update(double dt, const PacmanLevel& level, const Pacman& pacman, const std::vector<Ghost*>& ghosts);


private:
	void SetState(GhostAIState state);
	void ChangeTarget(const vec2& target);
	vec2 GetChaseTarget(const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost*>& ghosts);


	Ghost* mGhost;
	GhostName mName;
	vec2 mScatterTarget;
	vec2 mTarget;
	GhostAIState mState;
	GhostAIState mLastState;
	uint32_t mLookAheadDistance;
	uint64_t mTimer;



};