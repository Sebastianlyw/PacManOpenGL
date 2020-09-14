#pragma once
#include "Actor.h"

enum class GhostName : unsigned char
{
	RED			= 0,
	PINK		= 1,
	BLUE		= 2,
	NUM_GHOSTS  = 3
};

enum class GhostState :unsigned char
{
	GHOST_STATE_ALIVE			  = 0x01,
	GHOST_STATE_VULNERABLE		  = 0x02,
	GHOST_STATE_VULNERABLE_ENDING = 0x04,
	GHOST_STATE_DEAD			  = 0x08
};

class Pacman;


class GhostDelegate
{
public:
	virtual ~GhostDelegate() {}
	virtual void GhostDelegateGhostStateChangeTo(GhostState lastState, GhostState currentState) = 0;
	virtual void GhostWasReleasedFromHome() = 0;
	virtual void GhostWasResetToFirstPosition() = 0;
};


class Ghost : public Actor
{
public:

	Ghost():mState(GhostState::GHOST_STATE_ALIVE),mScore(0),mCanChangeDirection(false),mGhostTimer(0), mIsReleased(false),mGhostDelegate(nullptr) {}

	virtual void Init(const char* spritePath, const vec3& initialPos, float movementSpeed) override;
	virtual void SetMovementDirection(PacmanMovement direction) override;
	virtual void Stop() override;
	virtual void Draw(uint32_t dt) override;

	void Update(uint32_t dt, Pacman& pacman);
	void SetToVulnerable();
	void EatenByPacman();
	void ResetToSpwanPosition();

	inline bool IsDead() const { return mState == GhostState::GHOST_STATE_DEAD; }
	inline bool IsVulnerable() const { return mState == GhostState::GHOST_STATE_VULNERABLE; }
	inline void LockCanChangeDirection() { mCanChangeDirection = false; }
	inline bool CanChangeDirection() const { return mCanChangeDirection; }
	inline uint32_t GetScore() const { return mScore; }
	inline bool IsReleased() const { return mIsReleased; }
	void ReleaseFromHome();
	void SetGhostState(GhostState state);
	inline void SetGhostDelegate(GhostDelegate* delegate) { mGhostDelegate = delegate; }
private:

	friend class GhostAI;
	GhostState mState;
	double mGhostTimer;
	uint32_t mScore;
	bool mCanChangeDirection;
	bool mIsReleased;
	GhostDelegate* mGhostDelegate;
};