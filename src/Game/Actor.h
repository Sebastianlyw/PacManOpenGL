#pragma once

#include <glm/glm.hpp>
#include <string>
#include <stdint.h>
#include "..//Math/AARectangle.h"
#include "gameHelper.h"
#include "..//Graphics/Sprite.h"

using namespace glm;

class Actor
{
public:

	virtual ~Actor() { delete mSprite; }
	virtual void Init(const char* spritePath, const vec2& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement);
	virtual void Update(double dt);
	virtual void Draw(double dt);

	virtual void Stop();
	AARectangle GetEatingBoundingBox() const;

	inline void SetPositionX(float x) { mSprite->transformation.position.x = x; }
	inline void SetPositionY(float y) { mSprite->transformation.position.y = y; }
	inline glm::vec2 GetPosition() { return mSprite->transformation.position; }
	inline glm::mat4 Transformation() const { return mSprite->transformation.Get(); }

	inline bool IsFinishedAnimation() const { return false; }//mSprite.IsFinishedPlayingAnimation(); }
	inline const AARectangle GetBoundingBox() const { return mSprite->GetBoundingBox(); }
	inline void MoveBy(const vec2& delta) { mSprite->MoveBy(delta); }
	inline void MoveTo(const vec2& position) { mSprite->SetPosition(position); }
	inline vec2 Position() const { return mSprite->transformation.position; }
	inline PacmanMovement GetMovementDirection() const { return mMovementDirection; }
	inline void SetMovementSpeed(uint32_t movementSpeed) { mMovementSpeed = movementSpeed; }
	virtual inline void SetMovementDirection(PacmanMovement direction) { mMovementDirection = direction; }

protected:

	void AddAnimation(const char* animationName, bool looped);

	inline void ResetDelta() { mDelta = vec2(0.0f); }
	
	Sprite* mSprite;
private:
	vec2 mDelta;
	PacmanMovement mMovementDirection;
	uint32_t mMovementSpeed;
	bool mUpdateSpriteOnUpdate;
};

