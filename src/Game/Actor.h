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
	Actor():mDelta(vec2(0.0f)), mInitialPosition(vec3(0.0f)), mMovementDirection(PacmanMovement::PACMAN_MOVEMENT_NONE), mMovementSpeed(0), mPlayAnimation(false),mSprite(nullptr) {}
	virtual ~Actor() { delete mSprite; }
	virtual void Init(const char* spritePath, const vec3& initialPos, float movementSpeed);
	virtual void Update(uint32_t dt);
	virtual void Draw(uint32_t dt);

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
	inline void SetMovementSpeed(float movementSpeed) { mMovementSpeed = movementSpeed; }
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
	float mMovementSpeed;
	bool mPlayAnimation;
	
};

