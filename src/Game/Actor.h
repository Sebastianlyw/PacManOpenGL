#pragma once

#include <glm/glm.hpp>
#include <string>
#include <stdint.h>
#include "gameHelper.h"
#include "..//Math/AARectangle.h"
#include "..//Graphics/Sprite.h"
#include "../Graphics/Camera.h"

using namespace glm;

class Actor
{
public:

	virtual ~Actor() { delete mSprite; }
	virtual void Init(const char* spritePath, const vec3& initialPos, uint32_t movementSpeed);
	virtual void Update(double dt);
	virtual void Draw(double dt);

	virtual void Stop();
	virtual void ResetToSpwanPosition();
	AARectangle GetEatingBoundingBox() const;
	
	void SetTransformation(vec3 position, vec2 scale, float rotation) const;
	vec2 GetVelocity();
	inline vec2 Position() { return mSprite->transformation.position; }
	inline vec2 Size() { return mSprite->transformation.scale; }
	inline float Rotation() { return mSprite->transformation.rotation; }
	inline glm::mat4 GetTransformation() const { return mSprite->transformation.Get(); }
	inline bool IsFinishedAnimation() const { return false; }//mSprite.IsFinishedPlayingAnimation(); }
	inline const AARectangle GetBoundingBox() const { return mSprite->GetBoundingBox(); }
	inline void MoveBy(const vec2& delta) { mSprite->MoveBy(delta); }
	inline vec2 Position() const { return mSprite->transformation.position; }
	inline PacmanMovement GetMovementDirection() const { return mMovementDirection; }
	inline void SetMovementSpeed(uint32_t movementSpeed) { mMovementSpeed = movementSpeed; }
	inline void SetPlayAnimation(bool playAnimation) { mPlayAnimation = playAnimation; }
	virtual inline void SetMovementDirection(PacmanMovement direction) { mMovementDirection = direction; }
	inline Sprite* GetSpirte() { return mSprite; }

protected:

	void AddAnimation(const char* animationName, bool looped);

	inline void ResetDelta() { mDelta = vec2(0.0f); }
	
	Sprite* mSprite;
private:
	vec2 mDelta;
	vec3 mInitialPosition;
	PacmanMovement mMovementDirection;
	uint32_t mMovementSpeed;
	bool mPlayAnimation;
	
};

