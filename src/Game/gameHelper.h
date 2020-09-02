#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

static const uint32_t PACMAN_SPEED = 150; 
static const uint32_t PELLET_SIZE = 30;
static const uint32_t GHOST_MOVEMENT_SPEED =120;
static const uint32_t GHOST_MOVEMENT_SPEED_SLOW = 260;
static const uint32_t GHOST_VULNERABE_TIME = 6;
static const uint16_t MAX_LIVES = 4;
static const uint16_t GHOST_SCORE = 200;
static const uint16_t PELLET_SCORE = 10;
static const glm::uvec2 WINDOWSIZE = glm::uvec2(696, 848);
static const glm::uvec2 BACKGROUND_SIZE = glm::uvec2(696, 768);
static const glm::vec2 PACMAN_SIZE = glm::vec2(30.f, 30.f);
static const glm::mat4 MainCameraProjection = glm::ortho(0.0f, (float)WINDOWSIZE.x, (float)WINDOWSIZE.y, 0.0f, -1.0f, 1.0f);

static const uint32_t SCATTER_DURATION = 10;
static const glm::vec2 SCATTER_POS = glm::vec2(WINDOWSIZE.x - 70, 0);

enum PacmanMovement
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
