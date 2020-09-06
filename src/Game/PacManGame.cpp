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

	


PacmanGame::PacmanGame():mLives(MAX_LIVES), mGameState(ENTER_TO_START)
{

}
	
void PacmanGame::Init()
{
	//mAudioPlayer = createIrrKlangDevice();
	//mAudioPlayer->play2D("./assets/Audio/pacman_beginning.wav");
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
	pacManLive->Init("./assets/pacmanwalking.png", vec3(0.f), 0);

	mTextRender = new TextRenderer(WINDOWSIZE.x, WINDOWSIZE.y);
	mTextRender->Load("./assets/OCRAEXT.TTF",28);

	ResourceManager::LoadShader("./shaders/posteffect.vs", "./shaders/posteffect.fs", nullptr, "posteffect");
	ResourceManager::LoadShader("./shaders/skybox.vs", "./shaders/skybox.fs", nullptr, "skybox");
	mPostEffect = new PostEffectRender(ResourceManager::GetShader("posteffect"), WINDOWSIZE.x, WINDOWSIZE.y);

}
  
void PacmanGame::ResetGame()
{
	ResetLevel();
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

		if (mGhost->IsVulnerable())
		{
			mPacman->GetSpirte()->SetSize(vec2(PACMAN_SIZE.x * 1.25, PACMAN_SIZE.y * 1.25));
			if (mPacman->GetEatingBoundingBox().Intersects(mGhost->GetBoundingBox()))
			{
				AudioPlayer::instance().Play(AudioPlayer::EAT_GHOST, false);
				mGhost->EatenByPacman();
				mPacman->AteGhost(mGhost->GetScore());
			}
		}
		else if (mGhost->IsInvulnerable())
		{
			mPacman->GetSpirte()->SetSize(vec2(PACMAN_SIZE.x, PACMAN_SIZE.y ));
			if (mGhost->GetEatingBoundingBox().Intersects(mPacman->GetBoundingBox()))
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
		if (mLevel->IsLevelOver())
		{
			mGameState = GAME_WIN;
			ResetGame();
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
			mGameState = GAME_ALIVE;
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
		if (this->Keys[GLFW_KEY_C])
		{
			mPostEffect->Chaos = !mPostEffect->Chaos;
		}
		if (this->Keys[GLFW_KEY_V])
		{
			mPostEffect->Shake = !mPostEffect->Shake;
		}
		if (this->Keys[GLFW_KEY_B])
		{
			mPostEffect->Confuse = !mPostEffect->Confuse;
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
}

void PacmanGame::Render(float dt)
{
	
	mPostEffect->BeginRender();

	
	mPostEffect->EndRender();

	mPostEffect->Render(dt);
	//render postprocessing quad

	mLevel->Draw(dt);
	mPacman->Draw(dt);


	mGhost->Draw(dt);
	//Draw score
	std::stringstream my_ss;
	my_ss << this->mPacman->Score();
	mTextRender->Render("SCORE: " + my_ss.str(), WINDOWSIZE.x / 2 - 70, 10, 1.0f, glm::uvec3(0, 1, 1 ));

	//Render enter to start
	if (mGameState == ENTER_TO_START)
	{
		mTextRender->Render("Enter to Start", WINDOWSIZE.x / 2 - 100, WINDOWSIZE.y/2, 1.0f, glm::uvec3(1, 1, 0));
	}
	else if (mGameState == GAME_OVER)
	{
		mTextRender->Render("Game Over!!! ", WINDOWSIZE.x / 2 - 100, WINDOWSIZE.y / 2, 1.0f, glm::uvec3(1, 0.2, 0.2));
	}
	else if (mGameState == GAME_WIN)
	{
		mTextRender->Render("You Win the Game! Enter to restart.", 100, WINDOWSIZE.y / 2, 1.0f, glm::uvec3(1, 1, 0));
	}

	
	
	//Draw lives:
	for (int i = 0; i < this->mLives; i++)
	{
		pacManLive->SetTransformation(vec3(20 + i * 40, WINDOWSIZE.y - 40, 1), PACMAN_SIZE, 0);
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

