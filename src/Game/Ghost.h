#pragma once

#include "Actor.h"
#include "..//Utilities/Utils.h"
#include "..//Math/AARectangle.h"


enum GhostName
{
	RED = 0,
	BLUE,
	GREEN,
	NUM_GHOSTS
};

enum GhostState
{
	GHOST_STATE_ALIVE = 0,
	GHOST_STATE_VULNERABLE,
	GHOST_STATE_VULNERABLE_ENDING,
	GHOST_STATE_DEAD
};

class Ghost : public Actor
{


}