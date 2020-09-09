#include "gameHelper.h"
#include <Windows.h>

glm::vec2 GetMovementVector(PacmanMovement direction)
{
	if (direction == PacmanMovement::PACMAN_MOVEMENT_LEFT)
	{
		return glm::vec2(-1, 0);
	}
	else if (direction == PacmanMovement::PACMAN_MOVEMENT_DOWN)
	{
		return glm::vec2(0, 1);
	}
	else if (direction == PacmanMovement::PACMAN_MOVEMENT_RIGHT)
	{
		return glm::vec2(1, 0);
	}
	else if (direction == PacmanMovement::PACMAN_MOVEMENT_UP)
	{
		return glm::vec2(0, -1);
	}

	return glm::vec2(0.f, 0.f);
}

PacmanMovement GetOppositeDirection(PacmanMovement direction)
{
	if (direction == PacmanMovement::PACMAN_MOVEMENT_LEFT)
	{
		return PacmanMovement::PACMAN_MOVEMENT_RIGHT;
	}
	else if (direction == PacmanMovement::PACMAN_MOVEMENT_DOWN)
	{
		return PacmanMovement::PACMAN_MOVEMENT_UP;
	}
	else if (direction == PacmanMovement::PACMAN_MOVEMENT_RIGHT)
	{
		return PacmanMovement::PACMAN_MOVEMENT_LEFT;
	}
	else if (direction == PacmanMovement::PACMAN_MOVEMENT_UP)
	{
		return PacmanMovement::PACMAN_MOVEMENT_DOWN;
	}

	return PacmanMovement::PACMAN_MOVEMENT_NONE;
}

PacmanMovement GetDirectionFromVector(glm::vec2 vector)
{
	//since the origin is top left, reverse y for math caculation.
	double angle = glm::atan(-vector.y, vector.x);  //Range from -PI to PI
	constexpr double pi = glm::pi<double>();
	if ( (angle >= -pi / 4) && (angle < pi /4) )
	{
		return PacmanMovement::PACMAN_MOVEMENT_RIGHT;
	}
	else if ( (angle >= pi / 4) && (angle < 3 * pi / 4))
	{
		return PacmanMovement::PACMAN_MOVEMENT_UP;
	}
	else if ( (angle >= 3 * pi / 4 && angle < pi)  || (angle >= -pi && angle < -3 * pi / 4) )
	{
		return PacmanMovement::PACMAN_MOVEMENT_LEFT;
	}
	else if ( (angle >= -3 * pi / 4) && (angle < -pi / 4) )
	{
		return PacmanMovement::PACMAN_MOVEMENT_DOWN;
	}

	return PacmanMovement::PACMAN_MOVEMENT_NONE;
}

std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction)
{
	std::vector<PacmanMovement> perpendicularDirections;

	if (direction == PacmanMovement::PACMAN_MOVEMENT_DOWN || direction == PacmanMovement::PACMAN_MOVEMENT_UP)
	{
		perpendicularDirections.push_back(PacmanMovement::PACMAN_MOVEMENT_LEFT);
		perpendicularDirections.push_back(PacmanMovement::PACMAN_MOVEMENT_RIGHT);
	}
	else if (direction == PacmanMovement::PACMAN_MOVEMENT_RIGHT || direction == PacmanMovement::PACMAN_MOVEMENT_LEFT)
	{
		perpendicularDirections.push_back(PacmanMovement::PACMAN_MOVEMENT_UP);
		perpendicularDirections.push_back(PacmanMovement::PACMAN_MOVEMENT_DOWN);
	}
	else if (direction == PacmanMovement::PACMAN_MOVEMENT_NONE)
	{
		perpendicularDirections.push_back(PacmanMovement::PACMAN_MOVEMENT_UP);
		perpendicularDirections.push_back(PacmanMovement::PACMAN_MOVEMENT_LEFT);
		perpendicularDirections.push_back(PacmanMovement::PACMAN_MOVEMENT_DOWN);
		perpendicularDirections.push_back(PacmanMovement::PACMAN_MOVEMENT_RIGHT);
	}

	return perpendicularDirections;
}

std::vector<PacmanMovement> GetOtherDirections(PacmanMovement direction)
{
	std::vector<PacmanMovement> directions;

	for (int dir = (int)PacmanMovement::PACMAN_MOVEMENT_NONE + 1; dir <= (int)PacmanMovement::PACMAN_MOVEMENT_RIGHT; ++dir)
	{
		if (dir != (int)direction)
		{
			directions.push_back(static_cast<PacmanMovement>(dir));
		}
	}

	return directions;
}

std::string GetExeutablePath()
{
	TCHAR szPath[MAX_PATH];
	if (GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		return szPath;
	}
	return "";
}

