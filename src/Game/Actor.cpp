#include "Actor.h"
#include "..//Utilities/Utils.h"
#include <cmath>


void Actor::Init(const char* spritePath, const vec2& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement)
{
	mMovementDirection = PACMAN_MOVEMENT_NONE;
	mSprite = new Sprite(spritePath);
	mSprite->SetPosition(initialPos);

	mUpdateSpriteOnUpdate = updateSpriteOnMovement;
	mMovementSpeed = movementSpeed;
	mDelta = vec2(0.f);
}

void Actor::Update(double dt)
{
	if (mMovementDirection != PACMAN_MOVEMENT_NONE)
	{
		vec2 velocity = vec2(0.f);
		vec2 mv = GetMovementVector(mMovementDirection);
		velocity = vec2(mv.x * mMovementSpeed *dt, mv.y * mMovementSpeed *dt);
		mDelta += velocity;

		if (fabsf(mDelta.x) >= 1)
		{
			int dx = int(fabsf(mDelta.x));

			if (mDelta.x < 0)
			{
				mSprite->MoveBy(vec2(-dx, 0));
				mDelta.x += dx;
			}
			else
			{
				mSprite->MoveBy(vec2(dx, 0));
				mDelta.x -= dx;
			}
		}
		else if (fabsf(mDelta.y) >= 1)
		{
			int dy = int(fabsf(mDelta.y));

			if (mDelta.y < 0)
			{
				mSprite->MoveBy(vec2(0, -dy));
				mDelta.y += dy;
			}
			else
			{
				mSprite->MoveBy(vec2(0, dy));
				mDelta.y -= dy;
			}
		}

		mSprite->Update(dt);
	}

	if (mUpdateSpriteOnUpdate && mMovementDirection == PACMAN_MOVEMENT_NONE)
	{
		mSprite->Update(dt);
	}
}

void Actor::Draw(double dt)
{
	mSprite->draw(dt, AnimationType::Idle);
}

void Actor::Stop()
{
	SetMovementDirection(PACMAN_MOVEMENT_NONE);
	//mSprite->Stop();
}

//
//AARectangle Actor::GetEatingBoundingBox() const
//{
//	return AARectangle::Inset(GetBoundingBox(), Vec2D(3, 3));
//}

void Actor::AddAnimation(const char* animationName, bool looped)
{
	mSprite->AddAnimation(animationName, looped);
}


AARectangle Actor::GetEatingBoundingBox() const
{
	return AARectangle::Inset(GetBoundingBox(), vec2(10, 10));
}



