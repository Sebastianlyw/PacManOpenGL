#include "PacmanGame.h"
#include "Pacman.h"
#include "PacmanLevel.h"
#include "Ghost.h"
#include "GhostAI.h"
#include "..//Graphics/PostEffectRender.h"
#include "..//Utilities/AudioPlayer.h"

#include "../Graphics/Camera.h"
#include "../Utilities/resourceManager.h"
#include "../Graphics/shaderManager.h"
#include<iostream>
#include<sstream>
#include"..//Graphics/Sprite.h"
	

namespace
{
	const uint32_t RELEASE_GHOST_DURATION = 5000;
}
void PacmanGame::Init()
{
	mReleaseGhostTimer = 0;

	mLevel = new PacmanLevel();
	mLevel->Init("./assets/Pacman_level.txt");
	
	mPacman = new Pacman();
	mPacman->Init("./assets/pacmanwalking.png", mLevel->GetPacmanSpawnPosition() , PACMAN_SPEED);
	
	mGhosts.resize(size_t(GhostName::NUM_GHOSTS));
	mGhostAIs.resize(size_t(GhostName::NUM_GHOSTS));

	const vec2 RED_SCATTER_POS = vec2(WINDOWSIZE.x - 72, 0);
	const vec2 PINK_SCATTER_POS = vec2(WINDOWSIZE.x, WINDOWSIZE.y);
	const vec2 BLUE_SCATTER_POS = vec2(72, 0);

	//setup ghosts
	Ghost* redGhost = new Ghost();
	redGhost->Init("./assets/monster-red.png", mLevel->GetRedghostSpwanPosition(), GHOST_MOVEMENT_SPEED + 0.01);
	mGhosts[(int)(GhostName::RED)] = (redGhost);
	GhostAI* redGhostAI = new GhostAI();
	redGhostAI->Init(*redGhost, (uint32_t)redGhost->GetBoundingBox().GetWidth(), RED_SCATTER_POS, mLevel->GetPinkghostSpwanPosition(), mLevel->GetRedghostSpwanPosition(),  GhostName::RED);
	mGhostAIs[(int)(GhostName::RED)]= (redGhostAI);

	Ghost* pinkGhost = new Ghost();
	pinkGhost->Init("./assets/monster-pink.png", mLevel->GetPinkghostSpwanPosition(), GHOST_MOVEMENT_SPEED - 0.01);
	mGhosts[(int)GhostName::PINK]=(pinkGhost);
	GhostAI* pinkGhostAI = new GhostAI();
	pinkGhostAI->Init(*pinkGhost, (uint32_t)pinkGhost->GetBoundingBox().GetWidth(), PINK_SCATTER_POS, mLevel->GetPinkghostSpwanPosition(), mLevel->GetPinkghostSpwanPosition(), GhostName::PINK);
	mGhostAIs[(int)GhostName::PINK]=(pinkGhostAI);

	Ghost* blueGhost = new Ghost();
	blueGhost->Init("./assets/monster-blue.png", mLevel->GetBlueghostSpwanPosition(), GHOST_MOVEMENT_SPEED);
	mGhosts[(int)GhostName::BLUE] = (blueGhost);
	GhostAI* blueGhostAI = new GhostAI();
	blueGhostAI->Init(*blueGhost, (uint32_t)blueGhost->GetBoundingBox().GetWidth(), BLUE_SCATTER_POS, mLevel->GetPinkghostSpwanPosition(), mLevel->GetRedghostSpwanPosition(), GhostName::BLUE);
	mGhostAIs[(int)GhostName::BLUE] = (blueGhostAI);

	for (size_t i = 0; i <(int)GhostName::NUM_GHOSTS; ++i)
	{
		mGhosts[i]->SetGhostDelegate(mGhostAIs[i]);
	}

	pacManLive = new Pacman();
	pacManLive->Init("./assets/pacmanwalking.png", vec3(0.f), 0);

	mTextRender = new TextRenderer(WINDOWSIZE.x, WINDOWSIZE.y);
	mTextRender->Load("./assets/emulogic.TTF",28);
	ResourceManager::LoadShader("./shaders/particle.vs", "./shaders/particle.fs", nullptr, "test");
	mParticles = new ParticleRender(ResourceManager::GetShader("test"), "./assets/fire.png", 400);
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
	delete mParticles;
}

void PacmanGame::Update(uint32_t dt)
{
	//ToDo: Change to simple finite state machine. 
	if (mGameState == PacmanGameState::ENTER_TO_START)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			mGameState = PacmanGameState::GAME_ALIVE;
			ResetLevel();
		}
	}
	else if (mGameState == PacmanGameState::GAME_ALIVE)
	{
		UpdatePacmanMovement();
		mPacman->Update(dt);
		if (mPacman->IsSpeedUp())
		{
			mParticles->Update(dt, *mPacman, 2, 15);
		}
		mReleaseGhostTimer += dt;

		for (int i = 0; i < (int)GhostName::NUM_GHOSTS; i++)
		{
			Ghost& ghost = *mGhosts[i];
			GhostAI& ghostAI = *mGhostAIs[i];
		
			/*if (mReleaseGhostTimer >= RELEASE_GHOST_DURATION)
			{
				std::cout << "is At home: " << ghostAI.IsAtHome()<<std::endl;

				std::cout << "ghost.IsReleased " << ghost.IsReleased() << std::endl;
			}*/
			if (mReleaseGhostTimer >= RELEASE_GHOST_DURATION && ghostAI.IsAtHome() && !ghost.IsReleased())
			{
				mReleaseGhostTimer = 0;
				ghost.ReleaseFromHome(); 
			}

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
			else if (!ghost.IsDead() && ghost.GetEatingBoundingBox().Intersects(mPacman->GetBoundingBox()))//(ghost.IsInvulnerable())
			{
				mPacman->GetSpirte()->SetSize(vec2(PACMAN_SIZE.x, PACMAN_SIZE.y));
				if (ghost.GetEatingBoundingBox().Intersects(mPacman->GetBoundingBox()))
				{
					AudioPlayer::instance().Play(AudioPlayer::DEATH, false);
					mLives--;
					mPacman->EatenByGhost();
					mPressedDirection = PacmanMovement::PACMAN_MOVEMENT_NONE;
					mPacman->SetMovementDirection(mPressedDirection);
					mGameState = PacmanGameState::LOST_LIFE;
					return;
				}
			}
		}

		mLevel->Update(dt, *mPacman, mGhosts, mGhostAIs);
		if (mLevel->IsLevelOver())
		{
			mGameState = PacmanGameState::GAME_WIN;
		}

	}
	else if (mGameState == PacmanGameState::LOST_LIFE)
	{
		//ToDo: Render the death animation
		if (mLives > 0)
		{
			mGameState = PacmanGameState::ENTER_TO_START;
		}
		else
		{
			mGameState = PacmanGameState::GAME_OVER;
		}
	}	
	else if (mGameState == PacmanGameState::GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			ResetGame();
			mGameState = PacmanGameState::GAME_ALIVE;
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

	mGhosts[(int)(GhostName::RED)]->ReleaseFromHome();
}

void PacmanGame::InputUpdate()
{
	if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT])
	{
		mPressedDirection = PacmanMovement::PACMAN_MOVEMENT_LEFT;
	}
	if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT])
	{
		mPressedDirection = PacmanMovement::PACMAN_MOVEMENT_RIGHT;
	}
	if (this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP])
	{
		mPressedDirection = PacmanMovement::PACMAN_MOVEMENT_UP;
	}
	if (this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN])
	{
		mPressedDirection = PacmanMovement::PACMAN_MOVEMENT_DOWN;
	}
	
}

void PacmanGame::Render(uint32_t dt)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	glClearColor(0.4f, 0.2f, 0.2f, 1.0f);
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
	mTextRender->Render("SCORE: " + my_ss.str(), (float)(WINDOWSIZE.x / 2 - 150), 20, 1.25f, glm::uvec3(0.23,1,0.8));

	//Render enter to start
	if (mGameState == PacmanGameState::ENTER_TO_START)
	{
		mTextRender->Render("Enter to Start", (float)(WINDOWSIZE.x / 2 - 180), 200, 1.0f, glm::uvec3(0.2, 0.99, 1));
	}
	else if (mGameState == PacmanGameState::GAME_OVER)
	{
		mTextRender->Render("Game Over!!! ", (float)(WINDOWSIZE.x / 2 - 130), 200, 1.0f, glm::uvec3(1, 0.2, 0.2));
	}
	else if (mGameState == PacmanGameState::GAME_WIN)
	{
		mTextRender->Render("You Won the Game!", 120, 200, 1.0f, glm::uvec3(1, 0, 1));
	}

	//
	//
	//Draw lives:
	for (int i = 0; i < this->mLives; i++)
	{
		pacManLive->SetTransformation(vec3(50 + i * 40, WINDOWSIZE.y - 20, 1), PACMAN_SIZE, 0);
		pacManLive->Draw(0);
	}
	//
	if (mPacman->IsSpeedUp())
	{
		mParticles->Draw();
	}
	

}


void PacmanGame::UpdatePacmanMovement()
{
	if (mPressedDirection != PacmanMovement::PACMAN_MOVEMENT_NONE)
	{
		if (!mLevel->WillCollide(mPacman->GetBoundingBox(), mPressedDirection))
		{
			mPacman->SetMovementDirection(mPressedDirection);
		}
	}
}

