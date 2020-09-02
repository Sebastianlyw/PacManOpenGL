#include "gameHelper.h"

glm::vec2 GetMovementVector(PacmanMovement direction)
{
	if (direction == PACMAN_MOVEMENT_LEFT)
	{
		return glm::vec2(-1, 0);
	}
	else if (direction == PACMAN_MOVEMENT_DOWN)
	{
		return glm::vec2(0, 1);
	}
	else if (direction == PACMAN_MOVEMENT_RIGHT)
	{
		return glm::vec2(1, 0);
	}
	else if (direction == PACMAN_MOVEMENT_UP)
	{
		return glm::vec2(0, -1);
	}

	return glm::vec2(0.f, 0.f);
}

PacmanMovement GetOppositeDirection(PacmanMovement direction)
{
	if (direction == PACMAN_MOVEMENT_LEFT)
	{
		return PACMAN_MOVEMENT_RIGHT;
	}
	else if (direction == PACMAN_MOVEMENT_DOWN)
	{
		return PACMAN_MOVEMENT_UP;
	}
	else if (direction == PACMAN_MOVEMENT_RIGHT)
	{
		return PACMAN_MOVEMENT_LEFT;
	}
	else if (direction == PACMAN_MOVEMENT_UP)
	{
		return PACMAN_MOVEMENT_DOWN;
	}

	return PACMAN_MOVEMENT_NONE;
}

PacmanMovement GetDirectionFromVector(glm::vec2 vector)
{
	//since the origin is top left, reverse y for math caculation.
	double angle = glm::atan(-vector.y, vector.x);  //Range from -PI to PI
	double pi = glm::pi<double>();
	if ( (angle >= -pi / 4) && (angle < pi /4) )
	{
		return PACMAN_MOVEMENT_RIGHT;
	}
	else if ( (angle >= pi / 4) && (angle < 3 * pi / 4))
	{
		return PACMAN_MOVEMENT_UP;
	}
	else if ( (angle >= 3 * pi / 4 && angle < pi)  || (angle >= -pi && angle < -3 * pi / 4) )
	{
		return PACMAN_MOVEMENT_LEFT;
	}
	else if ( (angle >= -3 * pi / 4) && (angle < -pi / 4) )
	{
		return PACMAN_MOVEMENT_DOWN;
	}
	
}

std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction)
{
	std::vector<PacmanMovement> perpendicularDirections;

	if (direction == PACMAN_MOVEMENT_DOWN || direction == PACMAN_MOVEMENT_UP)
	{
		perpendicularDirections.push_back(PACMAN_MOVEMENT_LEFT);
		perpendicularDirections.push_back(PACMAN_MOVEMENT_RIGHT);
	}
	else if (direction == PACMAN_MOVEMENT_RIGHT || direction == PACMAN_MOVEMENT_LEFT)
	{
		perpendicularDirections.push_back(PACMAN_MOVEMENT_UP);
		perpendicularDirections.push_back(PACMAN_MOVEMENT_DOWN);
	}
	else if (direction == PACMAN_MOVEMENT_NONE)
	{
		perpendicularDirections.push_back(PACMAN_MOVEMENT_UP);
		perpendicularDirections.push_back(PACMAN_MOVEMENT_LEFT);
		perpendicularDirections.push_back(PACMAN_MOVEMENT_DOWN);
		perpendicularDirections.push_back(PACMAN_MOVEMENT_RIGHT);
	}

	return perpendicularDirections;
}

std::vector<PacmanMovement> GetOtherDirections(PacmanMovement direction)
{
	std::vector<PacmanMovement> directions;

	for (int dir = PacmanMovement::PACMAN_MOVEMENT_NONE + 1; dir <= PACMAN_MOVEMENT_RIGHT; ++dir)
	{
		if (dir != direction)
		{
			directions.push_back(static_cast<PacmanMovement>(dir));
		}
	}

	return directions;
}

