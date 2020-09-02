#include "PacmanGame.h"
#include "Pacman.h"
#include "PacmanLevel.h"
#include "Ghost.h"
#include "GhostAI.h"

#include<iostream>
#include<sstream>

PacmanGame::PacmanGame():mLives(MAX_LIVES), mGameState(ENTER_TO_START)
{

}

void PacmanGame::Init()
{	
	//level backgroud, pellet and fruit bonus item. 
	//use different shader for different postprocessing effect. 
	mLevel = new PacmanLevel();
	mLevel->Init("./assets/Pacman_level.txt");

	mPacman = new Pacman();
	mPacman->Init("./assets/pacmanwalking.png", mLevel->GetPacmanSpawnPosition() , PACMAN_SPEED);
	
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
	mLevel->ResetLevel();
}

PacmanGame::~PacmanGame()
{
	delete mPacman;
	delete mGhost;
	delete mGhostAI;
	delete pacManLive;
	delete mTextRender;
	delete mLevel;
}

void PacmanGame::Update(float dt)
{

	//ToDo: Change to simple finite state machine. 
	if (mGameState == ENTER_TO_START)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			mGameState = GAME_ALIVE;
			ResetLevel();
		}
	}
	else if (mGameState == GAME_ALIVE)
	{
		UpdatePacmanMovement();
		mPacman->Update(dt);
		mLevel->Update(dt, *mPacman, *mGhost);
		PacmanMovement dir = mGhostAI->Update(dt, *mLevel, *mPacman, *mGhost);
		if (dir != mGhost->GetMovementDirection())
		{
			mGhost->SetMovementDirection(dir);
			mGhost->LockCanChangeDirection();
		}

		mGhost->Update(dt, *mPacman);

		if (mGhost->IsVulnerable() && mPacman->GetEatingBoundingBox().Intersects(mGhost->GetBoundingBox()))
		{
			mGhost->EatenByPacman();
			mPacman->AteGhost(mGhost->GetScore());
		}
		else if (mGhost->IsInvulnerable() && mGhost->GetEatingBoundingBox().Intersects(mPacman->GetBoundingBox()))
		{
			mLives--;
			mPacman->EatenByGhost();
			mPressedDirection = PACMAN_MOVEMENT_NONE;
			mPacman->SetMovementDirection(mPressedDirection);
			mGameState = LOST_LIFE;
			return;
		}
		if (mLevel->IsLevelOver())
		{
			mGameState = ENTER_TO_START;
		}

	}
	else if (mGameState == LOST_LIFE)
	{
		//ToDo: Render the death animation
		if (mLives > 0)
		{
			mGameState = ENTER_TO_START;
		}
		else
		{
			mGameState = GAME_OVER;
		}
	}	
}

void PacmanGame::ResetLevel()
{
	mPacman->ResetToSpwanPosition();
	mPacman->ResetToFirstAnimation();
	mGhost->ResetToSpwanPosition();
}

void PacmanGame::InputUpdate(float dt)
{


	//if (this->State == GAME_ACTIVE)
	{

		if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT])
		{
			mPressedDirection = PACMAN_MOVEMENT_LEFT;
			mPacman->SetMovementDirection(PACMAN_MOVEMENT_LEFT);
		}
		if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT])
		{
			mPressedDirection = PACMAN_MOVEMENT_RIGHT;
			mPacman->SetMovementDirection(PACMAN_MOVEMENT_RIGHT);
		}
		if (this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP])
		{
			mPressedDirection = PACMAN_MOVEMENT_UP;
			mPacman->SetMovementDirection(PACMAN_MOVEMENT_UP);
		}
		if (this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN])
		{
			mPressedDirection = PACMAN_MOVEMENT_DOWN;
			mPacman->SetMovementDirection(PACMAN_MOVEMENT_DOWN);
		}
	}
}

void PacmanGame::Render(float dt)
{
	//glActiveTexture(GL_TEXTURE0);
	mPacman->Draw(dt);
	mLevel->Draw(dt);
	mGhost->Draw(dt);
	//Draw score
	std::stringstream my_ss;
	my_ss << this->mPacman->Score();
	mTextRender->Render("SCORE: " + my_ss.str(), WINDOWSIZE.x / 2 - 70, 10, 1.0f, glm::uvec3(0, 1, 1 ));

	//Render enter to start
	if (mGameState == ENTER_TO_START)
	{
		mTextRender->Render("Enter to Start", WINDOWSIZE.x / 2 - 70, WINDOWSIZE.y/2, 1.0f, glm::uvec3(0.2, 1, 1));
	}
	else if (mGameState == GAME_OVER)
	{
		mTextRender->Render("Game Over!!! ", WINDOWSIZE.x / 2 - 70, WINDOWSIZE.y / 2, 1.0f, glm::uvec3(1, 0.2, 0.2));
	}

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
		if (!mLevel->WillCollide(mPacman->GetBoundingBox(), mPressedDirection))
		{
			mPacman->SetMovementDirection(mPressedDirection);
		}
	}
}

