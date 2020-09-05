#pragma once
#include "Actor.h"

class Pacman : public Actor
{
public:
	virtual void Init(const char* spritePath, const vec3& initialPos, uint32_t movementSpeed) override;
	virtual void Update(double dt) override;
	virtual void SetMovementDirection(PacmanMovement movementDir) override;
	virtual void ResetToSpwanPosition() override;

	Pacman();
	void ResetToFirstAnimation();
	void EatenByGhost();
	void AteItem(uint32_t value);
	void AteGhost(uint32_t value);

	void Reset();
	inline void ResetScore() { mScore = 0; }
	inline void ResetGhostEatenMultiplier() { mGhostMultiplier = 1; }
	inline uint32_t Score() const { return mScore; }

private:
	void AddToScore(uint32_t value);

	uint32_t mScore;
	uint32_t mGhostMultiplier;
	bool mIsDying;//play dying animation
};

