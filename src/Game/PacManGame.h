#pragma once

#include  "gameHelper.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "..//Graphics/TextRender.h"
#include "..//Graphics/ParticleRender.h"

using namespace std;

//forward declartions:
class Sprite;
class Pacman;
class Ghost;
class PacmanLevel;
class GhostAI;

enum class PacmanGameState : unsigned char
{
	ENTER_TO_START	= 0x01,
	GAME_ALIVE		= 0x02,
	LOST_LIFE		= 0x04,
	GAME_OVER		= 0x08,
	GAME_WIN		= 0x10
};


class PacmanGame
{
public:
	PacmanGame() : mGameState(PacmanGameState::ENTER_TO_START), mLevel(nullptr), mLives(MAX_LIVES), mPacman(nullptr),
				   mParticles(nullptr), mPressedDirection(PacmanMovement::PACMAN_MOVEMENT_NONE),
		mTextRender(nullptr), pacManLive(nullptr){ }

	~PacmanGame();

	void Init();

	void Update(uint32_t dt);
	void InputUpdate();
	void UpdatePacmanMovement();

	void Render(uint32_t dt);

	bool Keys[1024];

private:

	void ResetGame();
	void ResetLevel();

	std::vector<GhostAI*> mGhostAIs;
	std::vector<Ghost*> mGhosts;
	Pacman* mPacman;
	Pacman* pacManLive;
	PacmanLevel* mLevel;
	PacmanMovement mPressedDirection;
	TextRenderer* mTextRender;
	uint16_t mLives;
	PacmanGameState mGameState;	
	ParticleRender* mParticles;
};
