#include "PacmanGame.h"
#include "gameHelper.h"
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

	pacManPlayer = new Pacman();
	pacManPlayer->Init("./assets/pacmanwalking.png", mLevel->GetPacmanSpawnPosition() , PACMAN_SPEED);
	pacManScore = new Pacman();
	pacManScore->Init("./assets/pacmanwalking.png", vec2(0,0), 0);


	mTextRender = new TextRenderer(WINDOWSIZE.x, WINDOWSIZE.y);
	mTextRender->Load("./assets/OCRAEXT.TTF",28);
}

void PacmanGame::ResetGame()
{
	mLives = MAX_LIVES;
}

PacmanGame::~PacmanGame()
{
	delete pacManPlayer;
	delete pacManScore;
	delete mTextRender;
	delete mLevel;
}

void PacmanGame::Update(float dt)
{
	UpdatePacmanMovement();
	pacManPlayer->Update(dt);
	mLevel->Update(dt, *pacManPlayer);
}

void PacmanGame::InputUpdate(float dt)
{
	//if (this->State == GAME_ACTIVE)
	{

		if (this->Keys[GLFW_KEY_A])
		{
			mPressedDirection = PACMAN_MOVEMENT_LEFT;
			pacManPlayer->SetMovementDirection(PACMAN_MOVEMENT_LEFT);
		}
		if (this->Keys[GLFW_KEY_D])
		{
			mPressedDirection = PACMAN_MOVEMENT_RIGHT;
			pacManPlayer->SetMovementDirection(PACMAN_MOVEMENT_RIGHT);
		}
		if (this->Keys[GLFW_KEY_W])
		{
			mPressedDirection = PACMAN_MOVEMENT_UP;
			pacManPlayer->SetMovementDirection(PACMAN_MOVEMENT_UP);
		}
		if (this->Keys[GLFW_KEY_S])
		{
			mPressedDirection = PACMAN_MOVEMENT_DOWN;
			pacManPlayer->SetMovementDirection(PACMAN_MOVEMENT_DOWN);
		}
	}
}

void PacmanGame::Render(float dt)
{
	//glActiveTexture(GL_TEXTURE0);
	pacManPlayer->Draw(dt);
	mLevel->Draw(dt);

	//Draw score
	std::stringstream my_ss;
	my_ss << this->pacManPlayer->Score();
	mTextRender->Render("SCORE: " + my_ss.str(), WINDOWSIZE.x / 2 - 70, 10, 1.0f, glm::uvec3(0, 1, 1 ));


	//Draw lives:
	for (int i = 0; i < this->mLives; i++)
	{
		pacManScore->SetTransformation(vec2(20 + i * 40, WINDOWSIZE.y - 40), PACMAN_SIZE, 0);
		pacManScore->Draw(0);
	}
	



}


void PacmanGame::UpdatePacmanMovement()
{
	if (mPressedDirection != PACMAN_MOVEMENT_NONE)
	{
		if (!mLevel->WillCollide(pacManPlayer->GetBoundingBox(), mPressedDirection))
		{
			pacManPlayer->SetMovementDirection(mPressedDirection);
		}
	}
}

