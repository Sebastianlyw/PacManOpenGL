#pragma once

#include <glm/glm.hpp>
#include <vector>

static const int PACMAN_SPEED = 200; //per second
static const int GHOST_MOVEMENT_SPEED = 45;
static const int GHOST_VULNERABLE_MOVEMENT_SPEED = 25;
static const int GHOST_BACK_TO_PEN_SPEED = 100;
static const glm::uvec2 WINDOWSIZE = glm::uvec2(928, 1024);
static const glm::uvec2 PACMAN_INITIAL_POSITION = glm::uvec2(40, 40);
static const glm::vec2 PACMAN_SIZE = glm::vec2(40.f, 40.f);


enum PacmanMovement
{
	PACMAN_MOVEMENT_NONE = 0,
	PACMAN_MOVEMENT_UP,
	PACMAN_MOVEMENT_LEFT,
	PACMAN_MOVEMENT_DOWN,
	PACMAN_MOVEMENT_RIGHT
};

glm::vec2 GetMovementVector(PacmanMovement direction);
PacmanMovement GetOppositeDirection(PacmanMovement direction);
std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction);
std::vector<PacmanMovement> GetOtherDirections(PacmanMovement direction);

