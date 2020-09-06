#pragma once
#include "Actor.h"

enum GhostName
{
	RED = 0,
	BLUE,
	GREEN,
	NUM_GHOSTS
};

enum GhostState
{
	GHOST_STATE_VULNERABLE = 0,
	GHOST_STATE_INVULNERABLE,
	GHOST_STATE_DEAD
};

class Pacman;

class Ghost : public Actor
{
public:

	virtual void Init(const char* spritePath, const vec3& initialPos, uint32_t movementSpeed) override;
	virtual void SetMovementDirection(PacmanMovement direction) override;
	virtual void Stop() override;
	virtual void Draw(double dt) override;

	void Update(double dt, Pacman& pacman);
	void SetToVulnerable();
	void EatenByPacman();
	void ResetToSpwanPosition();

	inline bool IsDead() const { return mState == GHOST_STATE_DEAD; }
	inline bool IsVulnerable() const { return mState == GHOST_STATE_VULNERABLE; }
	inline bool IsInvulnerable() const { return mState == GHOST_STATE_INVULNERABLE; }
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