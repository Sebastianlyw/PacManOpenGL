#include "PacmanGame.h"
#include "Pacman.h"
#include "PacmanLevel.h"
#include "Ghost.h"
#include "GhostAI.h"
#include "..//Graphics/PostEffectRender.h"
#include "..//Utilities/AudioPlayer.h"

#include<iostream>
#include<sstream>
#include"..//Graphics/Sprite.h"

#include <Windows.h>
#include <limits.h>

PacmanGame::PacmanGame():mLives(MAX_LIVES), mGameState(ENTER_TO_START)
{

}
	
void PacmanGame::Init()
{
	mLevel = new PacmanLevel();
	mLevel->Init("./assets/Pacman_level.txt");
	
	mPacman = new Pacman();
	mPacman->Init("./assets/pacmanwalking.png", mLevel->GetPacmanSpawnPosition() , PACMAN_SPEED);
	
	mGhosts.resize(NUM_GHOSTS);
	mGhostAIs.resize(NUM_GHOSTS);
	//setup ghosts
	Ghost* redGhost = new Ghost();
	redGhost->Init("./assets/monster-red.png", mLevel->GetRedghostSpwanPosition(), GHOST_MOVEMENT_SPEED + 5);
	mGhosts[RED] = (redGhost);
	GhostAI* redGhostAI = new GhostAI();
	redGhostAI->Init(*redGhost, redGhost->GetBoundingBox().GetWidth(), vec2(0), GhostName::RED);
	mGhostAIs[RED]= (redGhostAI);

	Ghost* pinkGhost = new Ghost();
	pinkGhost->Init("./assets/monster-pink.png", mLevel->GetPinkghostSpwanPosition(), GHOST_MOVEMENT_SPEED - 5);
	mGhosts[PINK]=(pinkGhost);
	GhostAI* pinkGhostAI = new GhostAI();
	pinkGhostAI->Init(*pinkGhost, pinkGhost->GetBoundingBox().GetWidth(), vec2(0), GhostName::PINK);
	mGhostAIs[PINK]=(pinkGhostAI);

	Ghost* blueGhost = new Ghost();
	blueGhost->Init("./assets/monster-blue.png", mLevel->GetBlueghostSpwanPosition(), GHOST_MOVEMENT_SPEED);
	mGhosts[BLUE] = (blueGhost);
	GhostAI* blueGhostAI = new GhostAI();
	blueGhostAI->Init(*blueGhost, blueGhost->GetBoundingBox().GetWidth(), vec2(0), GhostName::PINK);
	mGhostAIs[BLUE] = (blueGhostAI);

	pacManLive = new Pacman();
	pacManLive->Init("./assets/pacmanwalking.png", vec3(0.f), 0);

	mTextRender = new TextRenderer(WINDOWSIZE.x, WINDOWSIZE.y);
	mTextRender->Load("./assets/emulogic.TTF",28);
	ResourceManager::LoadShader("./shaders/skybox.vs", "./shaders/skybox.fs", nullptr, "skybox");
}
  
void PacmanGame::ResetGame()
{
	ResetLevel();
	mPacman->ResetScore();
	mLives = MAX_LIVES;
	mLevel->ResetLevel();
}

PacmanGame::~PacmanGame()
{
	for (auto g : mGhosts)
	{
		delete g;
	}
	mGhosts.clear();
	for (auto a : mGhostAIs)
	{
		delete a;
	}
	mGhostAIs.clear();

	delete mPacman;
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
		
		for (int i = 0; i < NUM_GHOSTS; i++)
		{
			Ghost& ghost = *mGhosts[i];
			GhostAI& ghostAI = *mGhostAIs[i];
			PacmanMovement dir = ghostAI.Update(dt, *mLevel, *mPacman, mGhosts);
			if (dir != ghost.GetMovementDirection())
			{
				ghost.SetMovementDirection(dir);
				ghost.LockCanChangeDirection();
			}

			ghost.Update(dt, *mPacman);

			if (ghost.IsVulnerable())
			{
				mPacman->GetSpirte()->SetSize(vec2(PACMAN_SIZE.x, PACMAN_SIZE.y));
				if (mPacman->GetEatingBoundingBox().Intersects(ghost.GetBoundingBox()))
				{
					AudioPlayer::instance().Play(AudioPlayer::EAT_GHOST, false);
					ghost.EatenByPacman();
					mPacman->AteGhost(ghost.GetScore());
				}
			}
			else if (ghost.IsInvulnerable())
			{
				mPacman->GetSpirte()->SetSize(vec2(PACMAN_SIZE.x, PACMAN_SIZE.y));
				if (ghost.GetEatingBoundingBox().Intersects(mPacman->GetBoundingBox()))
				{
					AudioPlayer::instance().Play(AudioPlayer::DEATH, false);
					mLives--;
					mPacman->EatenByGhost();
					mPressedDirection = PACMAN_MOVEMENT_NONE;
					mPacman->SetMovementDirection(mPressedDirection);
					mGameState = LOST_LIFE;
					return;
				}
			}
		}

		mLevel->Update(dt, *mPacman, mGhosts);
		if (mLevel->IsLevelOver())
		{
			mGameState = GAME_WIN;
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
	else if (mGameState == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			ResetGame();
			mGameState = GAME_ALIVE;
		}
	}
}

void PacmanGame::ResetLevel()
{
	mPacman->ResetToSpwanPosition();
	mPacman->ResetToFirstAnimation();
	for (auto g : mGhosts)
	{
		g->ResetToSpwanPosition();
	}
}

void PacmanGame::InputUpdate(float dt)
{
	if (this->Keys[GLFW_KEY_B])
	{
		mGameState = GAME_WIN;
	}
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

void PacmanGame::Render(float dt)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw level map and pellets. 
	ShaderManager shader = ResourceManager::GetShader("level");
	shader.Use().SetInteger("isSuperPacman", 0);
	for (auto ghost : mGhosts)
	{
		if (ghost->IsVulnerable())
		{
			shader.SetInteger("isSuperPacman", 1);
			break;
		}
	}
	

	mLevel->Draw(dt);
	mPacman->Draw(dt);

	for(auto ghost: mGhosts)
	{
		ghost->Draw(dt);
	}

	//Draw score
	std::stringstream my_ss; 
	my_ss << this->mPacman->Score();
	mTextRender->Render("SCORE: " + my_ss.str(), WINDOWSIZE.x / 2 - 150, 20, 1.25f, glm::uvec3(0.23,1,0.8));

	//Render enter to start
	if (mGameState == ENTER_TO_START)
	{
		mTextRender->Render("Enter to Start", WINDOWSIZE.x / 2 - 180, 200, 1.0f, glm::uvec3(0.2, 0.99, 1));
	}
	else if (mGameState == GAME_OVER)
	{
		mTextRender->Render("Game Over!!! ", WINDOWSIZE.x / 2 - 130, 200, 1.0f, glm::uvec3(1, 0.2, 0.2));
	}
	else if (mGameState == GAME_WIN)
	{
		mTextRender->Render("You Won the Game!", 120, 200, 1.0f, glm::uvec3(1, 0, 1));
	}

	
	
	//Draw lives:
	for (int i = 0; i < this->mLives; i++)
	{
		pacManLive->SetTransformation(vec3(50 + i * 40, WINDOWSIZE.y - 20, 1), PACMAN_SIZE, 0);
		pacManLive->Draw(0);
	}
	//
	

	

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

