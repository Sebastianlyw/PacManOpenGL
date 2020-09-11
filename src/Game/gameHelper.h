#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>


static const float PACMAN_SPEED_UP = 0.2f;
static const float PACMAN_SPEED = 0.14f;
static const uint32_t PELLET_SIZE = 30;
static const float GHOST_MOVEMENT_SPEED = 0.1f;
static const float GHOST_MOVEMENT_SPEED_SLOW = 0.05f;
static const uint16_t MAX_LIVES = 4;
static const uint16_t GHOST_SCORE = 200;
static const uint16_t PELLET_SCORE = 10;
static const glm::uvec2 WINDOWSIZE = glm::uvec2(696, 848);
static const glm::uvec2 BACKGROUND_SIZE = glm::uvec2(696, 768);
static const glm::uvec2 GHOST_RESPWAN_POSITION = glm::uvec2(320,380);
static const glm::vec2 PACMAN_SIZE = glm::vec2(30.f, 30.f);
static const uint32_t CHERRY_SCORE = 100;
static const uint32_t APPLE_SCORE = 100;


enum class PacmanMovement : uint8_t
{
	PACMAN_MOVEMENT_NONE = 0,
	PACMAN_MOVEMENT_UP,
	PACMAN_MOVEMENT_LEFT,
	PACMAN_MOVEMENT_DOWN,
	PACMAN_MOVEMENT_RIGHT
};

glm::vec2 GetMovementVector(PacmanMovement direction);
PacmanMovement GetDirectionFromVector(glm::vec2 vector);
PacmanMovement GetOppositeDirection(PacmanMovement direction);
std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction);
std::vector<PacmanMovement> GetOtherDirections(PacmanMovement direction);
std::string GetExeutablePath();