#pragma once
#include  "gameHelper.h"
#include "../Graphics/shaderManager.h"
#include "../Graphics/Camera.h"
#include "..//Graphics/TextRender.h"
#include "../Utilities/resourceManager.h"
#include "../Graphics/Sprite.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


using namespace std;

//forward declartions:
class Pacman;
class Ghost;
class PacmanLevel;
class GhostAI;
class PostEffectRender;
enum PacmanGameState
{
	ENTER_TO_START = 0,
	GAME_ALIVE,
	LOST_LIFE,
	GAME_OVER
};


class PacmanGame
{
public:
	PacmanGame();

	~PacmanGame();

	void Init();

	void Update(float dt);
	void InputUpdate(float dt);//vector<Action> actions);
	void UpdatePacmanMovement();

	void Render(float dt);

	bool Keys[1024];

private:

	void ResetGame();
	void ResetLevel();

	GhostAI* mGhostAI;
	Ghost* mGhost;
	Pacman* mPacman;
	Pacman* pacManLive;
	PacmanLevel* mLevel;
	PacmanMovement mPressedDirection;
	TextRenderer* mTextRender;
	uint16_t mLives;
	PacmanGameState mGameState;
	PostEffectRender* mPostEffect;
};