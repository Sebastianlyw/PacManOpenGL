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
	GHOST_STATE_VULNERABLE	 = 0x01,
	GHOST_STATE_INVULNERABLE = 0x02,
	GHOST_STATE_DEAD		 = 0x04
};

class Pacman;

class Ghost : public Actor
{
public:

	Ghost():mState(GhostState::GHOST_STATE_INVULNERABLE),mScore(0),mSpwanPos(vec2(0.f)),mCanChangeDirection(false),mGhostTimer(0) {}
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
	inline bool IsInvulnerable() const { return mState == GhostState::GHOST_STATE_INVULNERABLE; }
	inline void LockCanChangeDirection() { mCanChangeDirection = false; }
	inline bool CanChangeDirection() const { return mCanChangeDirection; }
	inline uint32_t GetScore() const { return mScore; }
private:

	void SetGhostState(GhostState state);
	GhostState mState;
	double mGhostTimer;
	uint32_t mScore;
	glm::vec2 mSpwanPos;
	bool mCanChangeDirection;
};