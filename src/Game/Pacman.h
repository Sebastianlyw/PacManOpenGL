#pragma once
#include "Actor.h"

class Pacman : public Actor
{
public:
	virtual void Init(const char* spritePath, const vec3& initialPos, float movementSpeed) override;
	virtual void Update(uint32_t dt) override;
	virtual void SetMovementDirection(PacmanMovement movementDir) override;
	virtual void ResetToSpwanPosition() override;
	virtual void Draw(uint32_t dt) override;

	Pacman();
	void ResetToFirstAnimation();
	void EatenByGhost();
	void AteItem(uint32_t value);
	void AteGhost(uint32_t value);

	void Reset();
	inline void ResetScore() { mScore = 0; }
	inline void ResetGhostEatenMultiplier() { mGhostMultiplier = 1; }
	inline uint32_t Score() const { return mScore; }
	inline bool IsSpeedUp() const { return mSpeedUp; }
	inline void SetSpeedUp(bool speedUp) { mSpeedUp = speedUp; }

private:
	void AddToScore(uint32_t value);

	uint32_t mScore;
	uint32_t mGhostMultiplier;
	bool mIsDying;//play dying animation
	bool mSpeedUp;
	double mSpeedUpTimer;
};

