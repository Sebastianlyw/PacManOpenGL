#include "Actor.h"
#include "..//Utilities/Utils.h"
#include "..//Utilities/resourceManager.h"
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Actor::Init(const char* spritePath, const vec3& initialPos, float movementSpeed)
{
	mInitialPosition = initialPos;
	mMovementDirection = PacmanMovement::PACMAN_MOVEMENT_NONE;
	mSprite = new Sprite(spritePath);
	mSprite->SetPosition(initialPos);
	mSprite->SetSize(PACMAN_SIZE);

	mMovementSpeed = movementSpeed;
	mDelta = vec2(0.f);
}

void Actor::Update(uint32_t dt)
{
	if (mMovementDirection != PacmanMovement::PACMAN_MOVEMENT_NONE)
	{
		vec2 velocity = vec2(0.f);
		vec2 mv = GetMovementVector(mMovementDirection);
		velocity = vec2(double(mv.x) * mMovementSpeed *dt, double(mv.y) * mMovementSpeed *dt);
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

	if (mMovementDirection == PacmanMovement::PACMAN_MOVEMENT_NONE)
	{
		mSprite->Update(dt);
	}
}

void Actor::Draw(uint32_t dt)
{
	ShaderManager shader = ResourceManager::GetShader("sprite");
	shader.Use().SetMatrix4("projection", Camera::instance().GetPerspectiveProjection());
	shader.SetMatrix4("view", Camera::instance().GetViewMatrix());
	shader.SetMatrix4("model_matrx", this->GetTransformation());
	mSprite->draw(dt, AnimationType::Walking);
}

void Actor::SetTransformation(vec3 position, vec2 scale, float rotation) const
{
	mSprite->transformation.position = position;
	mSprite->transformation.scale = scale;
	mSprite->transformation.rotation = rotation;
}

vec2 Actor::GetVelocity()
{
	vec2 v = GetMovementVector(mMovementDirection);
	v *= mMovementSpeed;
	return v;
}

void Actor::Stop()
{
	SetMovementDirection(PacmanMovement::PACMAN_MOVEMENT_NONE);
}


void Actor::ResetToSpwanPosition()
{
	mSprite->SetPosition(mInitialPosition);
	SetMovementDirection(PacmanMovement::PACMAN_MOVEMENT_NONE);
}

void Actor::AddAnimation(const char* animationName, bool looped)
{
	mSprite->AddAnimation(animationName, looped);
}


AARectangle Actor::GetEatingBoundingBox() const
{
	return AARectangle::Inset(GetBoundingBox(), vec2(10, 10));
}



