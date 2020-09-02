#include "PacmanGame.h"
#include "Pacman.h"
#include "PacmanLevel.h"
#include "Ghost.h"
#include "GhostAI.h"

#include<iostream>
#include<sstream>

PacmanGame::PacmanGame():mLives(MAX_LIVES)
{

}

void PacmanGame::Init()
{	
	//level backgroud, pellet and fruit bonus item. 
	//use different shader for different postprocessing effect. 
	mLevel = new PacmanLevel();
	mLevel->Init("./assets/Pacman_level.txt");

	mPacMan = new Pacman();
	mPacMan->Init("./assets/pacmanwalking.png", mLevel->GetPacmanSpawnPosition() , PACMAN_SPEED);
	
	mGhost = new Ghost();
	mGhost->Init("./assets/monster-red.png", mLevel->GetRedghostSpwanPosition(), GHOST_MOVEMENT_SPEED);

	mGhostAI = new GhostAI();
	mGhostAI->Init(*mGhost, mGhost->GetBoundingBox().GetWidth(), SCATTER_POS, GhostName::RED);

	pacManLive = new Pacman();
	pacManLive->Init("./assets/pacmanwalking.png", vec2(0,0), 0);

	mTextRender = new TextRenderer(WINDOWSIZE.x, WINDOWSIZE.y);
	mTextRender->Load("./assets/OCRAEXT.TTF",28);
}

void PacmanGame::ResetGame()
{
	mLives = MAX_LIVES;
}

PacmanGame::~PacmanGame()
{
	delete mPacMan;
	delete mGhost;
	delete mGhostAI;
	delete pacManLive;
	delete mTextRender;
	delete mLevel;
}

void PacmanGame::Update(float dt)
{
	UpdatePacmanMovement();
	mPacMan->Update(dt);
	mLevel->Update(dt, *mPacMan, *mGhost);
	
	//Ghost AI:
	PacmanMovement dir = mGhostAI->Update(dt, *mLevel, *mPacMan, *mGhost);
	if (dir != mGhost->GetMovementDirection())
	{
		mGhost->SetMovementDirection(dir);
		mGhost->LockCanChangeDirection();
	}

	mGhost->Update(dt, *mPacMan);
}

void PacmanGame::InputUpdate(float dt)
{
	//if (this->State == GAME_ACTIVE)
	{

		if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT])
		{
			mPressedDirection = PACMAN_MOVEMENT_LEFT;
			mPacMan->SetMovementDirection(PACMAN_MOVEMENT_LEFT);
		}
		if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT])
		{
			mPressedDirection = PACMAN_MOVEMENT_RIGHT;
			mPacMan->SetMovementDirection(PACMAN_MOVEMENT_RIGHT);
		}
		if (this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP])
		{
			mPressedDirection = PACMAN_MOVEMENT_UP;
			mPacMan->SetMovementDirection(PACMAN_MOVEMENT_UP);
		}
		if (this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN])
		{
			mPressedDirection = PACMAN_MOVEMENT_DOWN;
			mPacMan->SetMovementDirection(PACMAN_MOVEMENT_DOWN);
		}
	}
}

void PacmanGame::Render(float dt)
{
	//glActiveTexture(GL_TEXTURE0);
	mPacMan->Draw(dt);
	mLevel->Draw(dt);
	mGhost->Draw(dt);
	//Draw score
	std::stringstream my_ss;
	my_ss << this->mPacMan->Score();
	mTextRender->Render("SCORE: " + my_ss.str(), WINDOWSIZE.x / 2 - 70, 10, 1.0f, glm::uvec3(0, 1, 1 ));

	//Draw lives:
	for (int i = 0; i < this->mLives; i++)
	{
		pacManLive->SetTransformation(vec2(20 + i * 40, WINDOWSIZE.y - 40), PACMAN_SIZE, 0);
		pacManLive->Draw(0);
	}
	



}


void PacmanGame::UpdatePacmanMovement()
{
	if (mPressedDirection != PACMAN_MOVEMENT_NONE)
	{
		if (!mLevel->WillCollide(mPacMan->GetBoundingBox(), mPressedDirection))
		{
			mPacMan->SetMovementDirection(mPressedDirection);
		}
	}
}

