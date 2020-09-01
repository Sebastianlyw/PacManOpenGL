#include "Actor.h"
#include "..//Utilities/Utils.h"
#include "..//Utilities/resourceManager.h"
#include <cmath>


void Actor::Init(const char* spritePath, const vec2& initialPos, uint32_t movementSpeed)
{
	mMovementDirection = PACMAN_MOVEMENT_NONE;
	mSprite = new Sprite(spritePath);
	mSprite->SetPosition(initialPos);

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

	if (mMovementDirection == PACMAN_MOVEMENT_NONE)
	{
		mSprite->Update(dt);
	}
}

void Actor::Draw(double dt)
{
	ShaderManager shader = ResourceManager::GetShader("sprite");
	shader.Use().SetMatrix4("projection", MainCameraProjection);
	shader.SetMatrix4("model_matrx", this->GetTransformation());
	mSprite->draw(dt, AnimationType::Walking);
}

void Actor::SetTransformation(vec2 position, vec2 scale, float rotation) const
{
	mSprite->transformation.position = position;
	mSprite->transformation.scale = scale;
	mSprite->transformation.rotation = rotation;
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



