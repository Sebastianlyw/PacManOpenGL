#pragma once

#include <glm/glm.hpp>
#include "Line2D.h"

enum EdgeType
{
	BOTTOM_EDGE = 0,
	TOP_EDGE,
	LEFT_EDGE,
	RIGHT_EDGE,
	NUM_EDGES
};

static const glm::vec2 UP_DIR = { 0, -1 };
static const glm::vec2 DOWN_DIR = { 0, 1 };
static const glm::vec2 LEFT_DIR = { -1, 0 };
static const glm::vec2 RIGHT_DIR = { 1, 0 };

struct BoundaryEdge
{
	glm::vec2  normal;  //perpendicular to edge
	Line2D edge;
};
