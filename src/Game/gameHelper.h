#pragma once

#include <glm/glm.hpp>
#include <vector>

static const uint32_t PACMAN_SPEED = 200; 
static const uint32_t PELLET_SIZE = 30;
static const uint32_t GHOST_MOVEMENT_SPEED = 45;
static const uint32_t GHOST_VULNERABLE_MOVEMENT_SPEED = 25;
static const uint32_t GHOST_BACK_TO_PEN_SPEED = 100;
static const glm::uvec2 WINDOWSIZE = glm::uvec2(696, 828);
static const glm::uvec2 BACKGROUND_SIZE = glm::uvec2(696, 768);
static const glm::uvec2 PACMAN_INITIAL_POSITION = glm::uvec2(30, 80);
static const glm::vec2 PACMAN_SIZE = glm::vec2(30.f, 30.f);


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

