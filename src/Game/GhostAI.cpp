#include "GhostAI.h"
#include "PacmanLevel.h"
#include "Pacman.h"
#include <algorithm>

void GhostAI::Init(Ghost& ghost, uint32_t lookAheadDistance, const vec2& scatterTarget, GhostName name)
{
	mGhost = &ghost;
	mName = name;
	mLookAheadDistance = lookAheadDistance;
	mScatterTarget = scatterTarget;
	SetState(GhostAIState::GHOST_STATE_CHASE);
	mLastState = GhostAIState::GHOST_STATE_CHASE;

	//std::random_device randomDevice;
	//mRandomGenerator.seed(randomDevice());
}
PacmanMovement GhostAI::Update(double dt,const PacmanLevel& level, const Pacman& pacman, const std::vector<Ghost*>& ghosts)
{
	if (mGhost)
	{
		if (mState == GhostAIState::GHOST_STATE_START)
		{
			return PacmanMovement::PACMAN_MOVEMENT_NONE;
		}

		PacmanMovement currentDir = mGhost->GetMovementDirection();
		std::vector<PacmanMovement> tempDirections;
		std::vector<PacmanMovement> possibleDirections;

		possibleDirections = GetPerpendicularMovements(currentDir);

		if (currentDir != PacmanMovement::PACMAN_MOVEMENT_NONE)
		{
			possibleDirections.push_back(currentDir);
		}

		for (const auto& d : possibleDirections)
		{
			tempDirections.push_back(d);
		}

		possibleDirections.clear();

		for (const auto& d : tempDirections)
		{
			//If not collided with wall, add back to possible list. 
			if (!level.WillCollide(mGhost->GetBoundingBox(), d))
			{
				possibleDirections.push_back(d);
			}
		}

		std::sort(possibleDirections.begin(), possibleDirections.end(), [](const PacmanMovement& d1, const PacmanMovement& d2) {
			return d1 < d2;
		});

		if (mState == GhostAIState::GHOST_STATE_CHASE)
		{
			ChangeTarget(GetChaseTarget(pacman, level, ghosts));
		}

		
		PacmanMovement directionToGoIn = PacmanMovement::PACMAN_MOVEMENT_NONE;

		uint32_t lowestDistance = UINT32_MAX;

		//Get the moving direction for shotest distance option.	
		for (const auto& direction : possibleDirections)
		{
			
			vec2 movementVec = GetMovementVector(direction);
			movementVec *= mLookAheadDistance;

			AARectangle bbox = mGhost->GetBoundingBox();

			bbox.MoveBy(movementVec);

			uint32_t distanceToTarget = (uint32_t)glm::distance(bbox.GetCenterPoint(), mTarget);

			if (distanceToTarget < lowestDistance)
			{
				directionToGoIn = direction;
				lowestDistance = distanceToTarget;
			}
		}

//		assert(directionToGoIn != PACMAN_MOVEMENT_NONE);

		return directionToGoIn;
	}
	return PacmanMovement::PACMAN_MOVEMENT_NONE;
}



void GhostAI::SetState(GhostAIState state)
{
	if (mState == GhostAIState::GHOST_STATE_SCATTER || mState == GhostAIState::GHOST_STATE_CHASE)
	{
		mLastState = mState;
	}

	mState = state;

	switch (state) {
	case GhostAIState::GHOST_STATE_SCATTER:
		mTimer = 0;
		ChangeTarget(mScatterTarget);
		break;
	default:
		break;
	}
}

void GhostAI::ChangeTarget(const vec2& target)
{
	mTarget = target;
}


vec2 GhostAI::GetChaseTarget(const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost*>& ghosts)
{
	vec2 target = pacman.GetBoundingBox().GetCenterPoint();

	//ToDo: more AI for different ghosts.
	/*switch (mName)
	{
	case RED:
		return  pacman.GetBoundingBox().GetCenterPoint();
	case PINK:
	default:
		return  pacman.GetBoundingBox().GetCenterPoint();
	}*/

	if (mGhost->IsVulnerable())
	{
		target = vec2(WINDOWSIZE.x / 2, 0);
	}
	return target;
}
