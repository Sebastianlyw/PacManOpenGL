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

class GhostAI :public GhostDelegate
{

public :
	GhostAI():mGhost(nullptr), mName(GhostName::RED), mState(GhostAIState::GHOST_STATE_SCATTER), mLastState(GhostAIState::GHOST_STATE_SCATTER)
	, mScatterTarget(vec2(0.f)), mTarget(vec2(0.f)),mTimer(0), mLookAheadDistance(0){}

	void Init(Ghost& ghost, uint32_t lookAheadDistance, const vec2& scatterTarget,const vec2& homeTarget, const vec2& exitHomePosition, GhostName name);

	PacmanMovement Update(uint32_t dt, const PacmanLevel& level, const Pacman& pacman, const std::vector<Ghost*>& ghosts);

	virtual void GhostDelegateGhostStateChangeTo(GhostState lastState, GhostState currentState) override;
	virtual void GhostWasReleasedFromHome() override;
	virtual void GhostWasResetToFirstPosition() override;

	inline bool GoingToLeaveHome() const { return mState == GhostAIState::GHOST_STATE_EXIT_HOME; }
	inline bool IsAtHome()  const { return mState == GhostAIState::GHOST_STATE_AT_HOME || mState == GhostAIState::GHOST_STATE_START; }
	inline bool IsGoBackHome() const { return mState == GhostAIState::GHOST_STATE_GO_HOME; }
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
	std::default_random_engine mRandomGenerator;
	vec2 mExitHomePosition;
	vec2 mHomeTarget;

};