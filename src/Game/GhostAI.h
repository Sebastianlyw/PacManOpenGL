#pragma once

#include "Ghost.h"
#include "gameHelper.h"
#include <random>

class PacmanLevel;

enum GhostAIState
{
	GHOST_STATE_START = 0,
	GHOST_STATE_SCATTER,
	GHOST_STATE_CHASE,
	GHOST_STATE_AT_HOME,
	GHOST_STATE_EXIT_HOME,
	GHOST_STATE_GO_HOME
};

class GhostAI
{

public :
	GhostAI();

	void Init(Ghost& ghost, uint32_t lookAheadDistance, const vec2& scatterTarget, GhostName name);

	PacmanMovement Update(float dt, const PacmanLevel& level, const Pacman& pacman, const std::vector<Ghost*>& ghosts);


private:
	void SetState(GhostAIState state);
	void ChangeTarget(const vec2& target);
	vec2 GetChaseTarget(uint32_t dt, const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost*>& ghosts);


	Ghost* mGhost;
	GhostName mName;
	std::default_random_engine mRandomGenerator;
	vec2 mScatterTarget;
	vec2 mTarget;
	GhostAIState mState;
	GhostAIState mLastState;
	uint32_t mLookAheadDistance;
	uint64_t mTimer;



};