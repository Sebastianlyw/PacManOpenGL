#pragma once

#include "../Utilities/resourceManager.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/shaderManager.h"
#include "../Graphics/Camera.h"
#include "..//Graphics/TextRender.h"
#include "Pacman.h"
#include "PacmanLevel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;


// Initial velocity of the player paddle

enum Type
{
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	SHOOT
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

	Pacman* pacManPlayer;
	Pacman* pacManScore;
	PacmanLevel* mLevel;
	PacmanMovement mPressedDirection;
	TextRenderer* mTextRender;
	uint16_t mLives;
};