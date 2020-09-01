#include "PacmanGame.h"
#include "gameHelper.h"
#include<iostream>
#include<sstream>

PacmanGame::PacmanGame()
{

}

void PacmanGame::Init()
{
	/*resourceManager = make_unique<ResourceManager>();*/

	camera = make_unique<Camera>(WINDOWSIZE.x, WINDOWSIZE.y);
	mLevel.Init("./assets/Pacman_level.txt");
	
	//background = new Sprite("./assets/background.png");
//	background->transformation.scale = glm::vec2(800, 800);


	// load shaders
	ResourceManager::LoadShader("./shaders/sprite.vs", "./shaders/sprite.fs", nullptr, "sprite");

	//pacMan = new Pacman();
	pacManNew = new PacmanNew();
	pacManNew->Init("./assets/pacmanwalking.png",PACMAN_INITIAL_POSITION,PACMAN_SPEED,true);
	pacManNew->SetMovementSpeed(PACMAN_SPEED);
	
	mTextRender = new TextRenderer(WINDOWSIZE.x, WINDOWSIZE.y);
	mTextRender->Load("./assets/OCRAEXT.TTF",24);
//	ss << ;
	
}


PacmanGame::~PacmanGame()
{
	delete pacManNew;
	delete mTextRender;
}

void PacmanGame::Update(float dt)
{
	UpdatePacmanMovement();
	pacManNew->Update(dt);
	mLevel.Update(dt, *pacManNew);
}

void PacmanGame::InputUpdate(float dt)
{
	//if (this->State == GAME_ACTIVE)
	{
	
	

		if (this->Keys[GLFW_KEY_A])
		{
			mPressedDirection = PACMAN_MOVEMENT_LEFT;
			pacManNew->SetMovementDirection(PACMAN_MOVEMENT_LEFT);
		}
		if (this->Keys[GLFW_KEY_D])
		{
			mPressedDirection = PACMAN_MOVEMENT_RIGHT;
			pacManNew->SetMovementDirection(PACMAN_MOVEMENT_RIGHT);
		}
		if (this->Keys[GLFW_KEY_W])
		{
			mPressedDirection = PACMAN_MOVEMENT_UP;
			pacManNew->SetMovementDirection(PACMAN_MOVEMENT_UP);
		}
		if (this->Keys[GLFW_KEY_S])
		{
			mPressedDirection = PACMAN_MOVEMENT_DOWN;
			pacManNew->SetMovementDirection(PACMAN_MOVEMENT_DOWN);
		}
	}
}


void PacmanGame::UpdateViewport(glm::ivec2 aspectratio)
{
	camera->UpdateViewport(aspectratio.x, aspectratio.y);
}

void PacmanGame::Render(float dt)
{
	ShaderManager shader = ResourceManager::GetShader("sprite");
	shader.Use().SetMatrix4("projection", camera->Get_Projection());
	shader.SetMatrix4("model_matrx", pacManNew->Transformation());
	glActiveTexture(GL_TEXTURE0);
	pacManNew->Draw(dt);
	mLevel.Draw(dt);

	std::stringstream my_ss;
	my_ss << this->pacManNew->Score();
	string res = my_ss.str();
	mTextRender->RenderText("Scores: " + res, WINDOWSIZE.x / 4, 5, 1.0f, glm::uvec3(0, 1, 1));

//	shader.SetMatrix4("model_matrx", mLevel.GetBackground()->transformation.Get());
	//mLevel.GetBackground()->draw(0, AnimationType::Idle);

	//shader.SetMatrix4("model_matrx", mLevel.GetBackground()->transformation.Get());
	//mLevel.GetBackground()->draw(0, AnimationType::Idle);

	//shader.SetMatrix4("model_matrx", pacMan->Transformation());
//	pacMan->draw(dt);

	//To Do , draw level contents: like  pellets  and power ups. 


	//Shaders->Textured_Shader()->use();
	//Shaders->Textured_Shader()->Send_Mat4("projection", );


	//Shaders->Textured_Shader()->Send_Mat4("model_matrx", background->transformation());
	

}


void PacmanGame::UpdatePacmanMovement()
{
	if (mPressedDirection != PACMAN_MOVEMENT_NONE)
	{
		if (!mLevel.WillCollide(pacManNew->GetBoundingBox(), mPressedDirection))
		{
			pacManNew->SetMovementDirection(mPressedDirection);
		}
	}
}

