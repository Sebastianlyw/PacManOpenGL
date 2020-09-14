#include "GhostAI.h"
#include "PacmanLevel.h"
#include "Pacman.h"
#include <algorithm>
#include <cassert>

namespace {
	const uint32_t HOME_WAIT_DURATION = 6000;
	const uint32_t SCATTER_MODE_DURATION = 10000;
}


void GhostAI::Init(Ghost& ghost, uint32_t lookAheadDistance, const vec2& scatterTarget, const vec2& homeTarget, const vec2& exitHomePosition, GhostName name)
{
	mGhost = &ghost;
	mName = name;
	mLookAheadDistance = lookAheadDistance;
	mScatterTarget = scatterTarget;
	SetState(GhostAIState::GHOST_STATE_AT_HOME);
	mLastState = GhostAIState::GHOST_STATE_AT_HOME;
	mTimer = 0;
	std::random_device randomDevice;
	mRandomGenerator.seed(randomDevice());
	mHomeTarget = homeTarget;
	mExitHomePosition = exitHomePosition;
}

PacmanMovement GhostAI::Update(uint32_t dt,const PacmanLevel& level, const Pacman& pacman, const std::vector<Ghost*>& ghosts)
{
	if (mGhost)
	{
		if (mState == GhostAIState::GHOST_STATE_START)
		{
			return PacmanMovement::PACMAN_MOVEMENT_NONE;
		}
		
		if (mState == GhostAIState::GHOST_STATE_AT_HOME)
		{
			mTimer += dt;

			if (mTimer > HOME_WAIT_DURATION)
			{
				SetState(GhostAIState::GHOST_STATE_EXIT_HOME);
			}
			return PacmanMovement::PACMAN_MOVEMENT_NONE;
		}

		if (mState == GhostAIState::GHOST_STATE_GO_HOME && mGhost->Position() == mHomeTarget)
		{
			SetState(GhostAIState::GHOST_STATE_AT_HOME);
			mGhost->SetGhostState(GhostState::GHOST_STATE_ALIVE);
			return PacmanMovement::PACMAN_MOVEMENT_NONE;
		}
		
		if (mState == GhostAIState::GHOST_STATE_EXIT_HOME && fabs(mGhost->Position().x - mExitHomePosition.x) <30)
		{
			SetState(GhostAIState::GHOST_STATE_SCATTER);
		}		
		
		if (mState == GhostAIState::GHOST_STATE_SCATTER)
		{
			mTimer += dt;
			if (mTimer > SCATTER_MODE_DURATION)
			{
				SetState(GhostAIState::GHOST_STATE_CHASE);
			}
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
			if (!level.WillCollide(*mGhost, *this, d))
			{
				possibleDirections.push_back(d);
			}
		}

		std::sort(possibleDirections.begin(), possibleDirections.end(), [](const PacmanMovement& d1, const PacmanMovement& d2) {
			return d1 < d2;
		});

		if (mGhost->IsVulnerable() && (mState == GhostAIState::GHOST_STATE_CHASE || mState == GhostAIState::GHOST_STATE_SCATTER))
		{
			std::uniform_int_distribution<size_t> distribution(0, possibleDirections.size() - 1);
			return possibleDirections[static_cast<int>(distribution(mRandomGenerator))];
		}

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

		if (directionToGoIn == PacmanMovement::PACMAN_MOVEMENT_NONE)
		{
			directionToGoIn = GetOppositeDirection(currentDir);
		}

	//	if (directionToGoIn == PacmanMovement::PACMAN_MOVEMENT_NONE)
		{
	//		return GetOppositeDirection(currentDir);
		}
		//assert(directionToGoIn != PacmanMovement::PACMAN_MOVEMENT_NONE);

		return directionToGoIn;
	}
	return PacmanMovement::PACMAN_MOVEMENT_NONE;
}

void GhostAI::GhostDelegateGhostStateChangeTo(GhostState lastState, GhostState currentState)
{
	if (mGhost && mGhost->IsReleased() 
		&& (lastState == GhostState::GHOST_STATE_VULNERABLE || lastState == GhostState::GHOST_STATE_VULNERABLE_ENDING)
		&& (IsAtHome() || GoingToLeaveHome()))
	{
		mGhost->SetMovementDirection(GetOppositeDirection(mGhost->GetMovementDirection()));
	}

	if (currentState == GhostState::GHOST_STATE_DEAD)
	{
		SetState(GhostAIState::GHOST_STATE_GO_HOME);
	}
	else if ((lastState == GhostState::GHOST_STATE_VULNERABLE || lastState == GhostState::GHOST_STATE_VULNERABLE_ENDING) && currentState == GhostState::GHOST_STATE_ALIVE)
	{
		if (mState == GhostAIState::GHOST_STATE_CHASE || mState == GhostAIState::GHOST_STATE_SCATTER)
		{
			SetState(mLastState);
		}
	}
}

void GhostAI::GhostWasReleasedFromHome()
{
	if (mState == GhostAIState::GHOST_STATE_START)
	{
		SetState(GhostAIState::GHOST_STATE_EXIT_HOME);
	}
}

void GhostAI::GhostWasResetToFirstPosition()
{
	SetState(GhostAIState::GHOST_STATE_START);
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
	case GhostAIState::GHOST_STATE_AT_HOME:
		mTimer = 0;
		break;
	case GhostAIState::GHOST_STATE_EXIT_HOME: 
		ChangeTarget(mExitHomePosition);
		break;
	case GhostAIState::GHOST_STATE_GO_HOME:
		{
			vec2 target = vec2(mHomeTarget.x + mGhost->GetBoundingBox().GetWidth() / 2, mHomeTarget.y + mGhost->GetBoundingBox().GetHeight() / 2); 
			ChangeTarget(target);
		}
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
	switch (mName)
	{
		case GhostName::RED:
			target =  pacman.GetBoundingBox().GetCenterPoint();
			break;
		case GhostName::PINK:
			target = pacman.GetBoundingBox().GetCenterPoint() + 2.f * GetMovementVector(pacman.GetMovementDirection()) * pacman.GetBoundingBox().GetWidth();
			
			break;
		case GhostName::BLUE:
			{
				vec2 pacmanOffsetPoint = pacman.GetBoundingBox().GetCenterPoint() + (GetMovementVector(pacman.GetMovementDirection()) * pacman.GetBoundingBox().GetWidth());
				target = (pacmanOffsetPoint - ghosts[static_cast<int>(GhostName::RED)]->GetBoundingBox().GetCenterPoint()) * 2.f + ghosts[static_cast<int>(GhostName::RED)]->GetBoundingBox().GetCenterPoint();
			}
			
			break;
	}


	//if (mGhost->IsVulnerable())
	//{
	//	target = vec2(WINDOWSIZE.x / 2, 0);
	//}
	return target;
}
