#include "PacmanLevel.h"
#include "..//Utilities/FileCommandLoader.h"
#include "..//Graphics/ShaderManager.h"
#include "..//Utilities/resourceManager.h"
#include "gameHelper.h"
#include "PacManGame.h"
#include "Pacman.h"
#include "Ghost.h"
#include "../Utilities/AudioPlayer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "..//Graphics/Camera.h"
#include <string>


namespace {
	const float SUPER_PELLET_OFFSET = 12;
	const uint32_t BONUS_DURATION = 13000;
	const vec2 LEVEL_MAP_OFFSET = vec2(10, 50);
}

PacmanLevel::~PacmanLevel()
{
	delete mBackground;
	delete mSkybox;
	delete mPelletSprite;
	delete mCherrySprite;
	delete mAppleSprite;
}

bool PacmanLevel::Init(const std::string& levelPath) 
{
	ResourceManager::LoadShader("./shaders/level.vs", "./shaders/level.fs", nullptr, "level");
	AudioPlayer::instance().Play(AudioPlayer::BACKGROUND, true);
	
	bool levelLoaded = LoadLevel(levelPath);
	if (levelLoaded)
	{
		ResetLevel();
	}
	return levelLoaded;
}


void PacmanLevel::Update(uint32_t dt, Pacman& pacman, std::vector<Ghost*>& ghosts, std::vector<GhostAI*>& ghostAIs)
{
	BoundaryEdge edge;
	//Collision checking game logic here. 
	for (const auto & wall : mWalls)
	{
		if (wall.HasCollided(pacman.GetBoundingBox(), edge))
		{
			vec2 offset = wall.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (auto& ghost : ghosts)
		{
			if (wall.HasCollided(ghost->GetBoundingBox(), edge))
			{
				vec2 offset = wall.GetCollisionOffset(ghost->GetBoundingBox());
				ghost->MoveBy(offset);
				ghost->Stop();
			}
		}
	}

	for (const auto& gate : mGate)
	{
		if (gate.HasCollided(pacman.GetBoundingBox(), edge))
		{
			vec2 offset = gate.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (size_t i = 0; i < static_cast<int>(GhostName::NUM_GHOSTS); ++i)
		{
			GhostAI& ghostAI = *ghostAIs[i];
			if (!(ghostAI.GoingToLeaveHome() || ghostAI.IsGoBackHome()) && gate.HasCollided(ghosts[i]->GetBoundingBox(), edge))
			{
				vec2 offset = gate.GetCollisionOffset(ghosts[i]->GetBoundingBox());
				ghosts[i]->MoveBy(offset);
				ghosts[i]->Stop();
			}
		}
	}

	for (auto& pellet : mPellets)
	{
		if (!pellet.eaten)
		{
			if (pacman.GetEatingBoundingBox().Intersects(pellet.mBBox))
			{
				pellet.eaten = true;
				pacman.AteItem(pellet.score);
				AudioPlayer::instance().Play(AudioPlayer::CHOMP, false);

				if (pellet.powerPellet)
				{
					AudioPlayer::instance().Play(AudioPlayer::EAT_FRUIT, false);
					pacman.ResetGhostEatenMultiplier();
				
					for (auto& ghost : ghosts)
					{
						if (!ghost->IsDead())
						{
							ghost->SetToVulnerable();
						}
					}
				}
			}
		}

		if (pellet.powerPellet && pellet.eaten)
		{
			mPowerTimer += dt;
			if (mPowerTimer > BONUS_DURATION)
			{
				mPowerTimer = 0;
				pellet.eaten = false;
			}
		}
	}

	for (int i = 0; i < mFruits.size(); i++)
	{
		if (pacman.GetEatingBoundingBox().Intersects(mFruits[i].mBBox) && !mFruits[i].eaten)
		{
			mFruits[i].eaten = true;
			pacman.AteItem(mFruits[i].score);
			AudioPlayer::instance().Play(AudioPlayer::EAT_FRUIT, false);
			if (mFruits[i].name == FRUITE_NAME::APPLE)
			{
				pacman.SetSpeedUp(true);
				pacman.SetMovementSpeed(PACMAN_SPEED_UP);
			}
		}
		if (mFruits[i].eaten)
		{
			mBonusTimer[i] += dt;
			if (mBonusTimer[i] > BONUS_DURATION)
			{
				mBonusTimer[i] = 0;
				mFruits[i].eaten = false;
			}
		}
	}
	

}

void PacmanLevel::Draw(uint32_t dt)
{
	ShaderManager shader = ResourceManager::GetShader("level");

	// pass projection matrix to shader (note that in this case it could change every frame)
	shader.Use().SetMatrix4("projection", Camera::instance().GetPerspectiveProjection());
	shader.SetMatrix4("view", Camera::instance().GetViewMatrix());
	
	shader.SetInteger("isSkyBox", 1);
	mShaderTimer += dt/80000.f;
	if (mShaderTimer >= 1)  // used as offset to uv y value.  range from 0-1
	{
		mShaderTimer = 0;
	}
	shader.SetFloat("deltaTime", mShaderTimer);
	shader.SetMatrix4("model_matrx", mSkybox->transformation.Get());
	mSkybox->draw(0);
	shader.SetFloat("isSkyBox", 0 );
	shader.SetMatrix4("model_matrx", mBackground->transformation.Get());
	
	float timeValue = (float)glfwGetTime();
	float greenValue = (sin(timeValue * 10) / 2.0f) + 0.5f;
	shader.Use().SetFloat("sinColor", greenValue);
	mBackground->draw(0);
	

	for (const auto& pellet : mPellets)
	{
		if (!pellet.eaten)
		{
			mPelletSprite->transformation.position = vec3(pellet.mBBox.GetCenterPoint().x -5, pellet.mBBox.GetCenterPoint().y - 5,1);
			if (pellet.powerPellet) {
				mPelletSprite->transformation.position.y -= SUPER_PELLET_OFFSET;
				mPelletSprite->transformation.scale = vec2(PELLET_SIZE * 2, PELLET_SIZE * 2);
			}
			else
			{
				mPelletSprite->transformation.scale = vec2(PELLET_SIZE, PELLET_SIZE);
			}
			shader.SetMatrix4("model_matrx", mPelletSprite->transformation.Get());
	
			mPelletSprite->draw(0);
		}
	}

	for (auto fruit : mFruits)
	{
		if (fruit.eaten)
		{
			continue;
		}

		if (fruit.name == FRUITE_NAME::CHERRY)
		{
			mCherrySprite->SetPosition(vec3(fruit.position, 0));
			shader.SetMatrix4("model_matrx", mCherrySprite->transformation.Get());
			mCherrySprite->draw(0);
		}
		else if (fruit.name == FRUITE_NAME::APPLE)
		{
			mAppleSprite->SetPosition(vec3(fruit.position, 0));
			shader.SetMatrix4("model_matrx", mAppleSprite->transformation.Get());
			mAppleSprite->draw(0);
		}
	}
}

bool PacmanLevel::LoadLevel(const std::string& path) 
{
	FileCommandLoader fileLoader;
	std::string imageName;

	Command bgImageCommand;
	bgImageCommand.command = "bg_image";
	bgImageCommand.parseFunc = [this, &imageName](ParseFuncParams params)
	{
		imageName = FileCommandLoader::ReadString(params);
		mBackground = new Sprite(("./" + std::string("assets/") + imageName).c_str());
		mBackground->transformation.scale = glm::vec2(BACKGROUND_SIZE.x, BACKGROUND_SIZE.y);
		mBackground->transformation.position = glm::vec3(LEVEL_MAP_OFFSET,0);
		assert(mBackground->IsLoaded() && "Didn't load the bg image");
	};
	fileLoader.AddCommand(bgImageCommand);

	Command pelletImageCommand;
	pelletImageCommand.command = "pellet_image";
	pelletImageCommand.parseFunc = [this, &imageName](ParseFuncParams params)
	{
		imageName = FileCommandLoader::ReadString(params);
		mPelletSprite = new Sprite(("./" + std::string("assets/") + imageName).c_str());
		mPelletSprite->transformation.scale = glm::vec2(PELLET_SIZE, PELLET_SIZE);
		assert(mPelletSprite->IsLoaded() && "Didn't load the pellet image");
	};
	fileLoader.AddCommand(pelletImageCommand);


	Command cherryImageCommand;
	cherryImageCommand.command = "cherry_image";
	cherryImageCommand.parseFunc = [this, &imageName](ParseFuncParams params)
	{
		imageName = FileCommandLoader::ReadString(params);
		mCherrySprite = new Sprite(("./" + std::string("assets/") + imageName).c_str());
		mCherrySprite->transformation.scale = PACMAN_SIZE;
		assert(mCherrySprite->IsLoaded() && "Didn't load the cherry image");
	};
	fileLoader.AddCommand(cherryImageCommand);

	Command appleImageCommand;
	appleImageCommand.command = "apple_image";
	appleImageCommand.parseFunc = [this, &imageName](ParseFuncParams params)
	{
		imageName = FileCommandLoader::ReadString(params);
		mAppleSprite = new Sprite(("./" + std::string("assets/") + imageName).c_str());
		mAppleSprite->transformation.scale = PACMAN_SIZE;
		assert(mAppleSprite->IsLoaded() && "Didn't load the cherry image");
	};
	fileLoader.AddCommand(appleImageCommand);

	Command tileWidthCommand;
	tileWidthCommand.command = "tile_width";
	tileWidthCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().width = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileWidthCommand);

	Command tileHeightCommand;
	tileHeightCommand.command = "tile_height";
	tileHeightCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTileHeight = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileHeightCommand);

	Command tileCommand;
	tileCommand.command = "tile";
	tileCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.push_back(Tile());
	};
	fileLoader.AddCommand(tileCommand);

	Command tileSymbolCommand;
	tileSymbolCommand.command = "tile_symbol";
	tileSymbolCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().symbol = FileCommandLoader::ReadChar(params);
	};
	fileLoader.AddCommand(tileSymbolCommand);

	Command tileCollisionCommand;
	tileCollisionCommand.command = "tile_collision";
	tileCollisionCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isCollidable = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tileCollisionCommand);

	Command tileOffsetCommand;
	tileOffsetCommand.command = "tile_offset";
	tileOffsetCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().offset = FileCommandLoader::ReadSize(params);
	};
	fileLoader.AddCommand(tileOffsetCommand);

	Command tileExcludePelletCommand;
	tileExcludePelletCommand.command = "tile_exclude_pellet";
	tileExcludePelletCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isExcludePelletTile = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tileExcludePelletCommand);

	Command tilePacmanSpawnPointCommand;
	tilePacmanSpawnPointCommand.command = "tile_pacman_spawn";
	tilePacmanSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isPacmanSpawnTile = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tilePacmanSpawnPointCommand);

	Command tileRedghostSpawnPointCommand;
	tileRedghostSpawnPointCommand.command = "tile_redghost_spawn";
	tileRedghostSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isRedGhostSpawnTile = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tileRedghostSpawnPointCommand);
	
	Command tilePinkghostSpawnPointCommand;
	tilePinkghostSpawnPointCommand.command = "tile_pinkghost_spawn";
	tilePinkghostSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isPinkGhostSpawnTile = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tilePinkghostSpawnPointCommand);

	Command tileBlueghostSpawnPointCommand;
	tileBlueghostSpawnPointCommand.command = "tile_blueghost_spawn";
	tileBlueghostSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isBlueGhostSpwanTile = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tileBlueghostSpawnPointCommand);


	Command tileCherrySpawnPointCommand;
	tileCherrySpawnPointCommand.command = "tile_cherry_spawn";
	tileCherrySpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isCherrySpwanTile = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tileCherrySpawnPointCommand);

	Command tileAppleSpawnPointCommand;
	tileAppleSpawnPointCommand.command = "tile_apple_spawn";
	tileAppleSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isAppleSpwanTile = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tileAppleSpawnPointCommand);

	Command tileGateCommand;
	tileGateCommand.command = "tile_is_gate";
	tileGateCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isGate = FileCommandLoader::ReadInt(params) > 0;
	};
	fileLoader.AddCommand(tileGateCommand);

	glm::vec2 layoutOffset;
	Command layoutOffsetCommand;
	layoutOffsetCommand.command = "layout_offset";
	layoutOffsetCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params)
	{
		mLayoutOffset = FileCommandLoader::ReadSize(params);
		layoutOffset = mLayoutOffset;
	};
	fileLoader.AddCommand(layoutOffsetCommand);

	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = CommandType::COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params)
	{
		int startingX = (int)layoutOffset.x;

		for (int c = 0; c < params.line.length(); ++c)
		{
			Tile* tile = GetTileBySymbol(params.line[c]);

			if (tile)
			{
				tile->position = glm::vec2(startingX, layoutOffset.y);

				if (tile->isCollidable)
				{
					Excluder wall;
					wall.Init(AARectangle(glm::vec2(startingX, layoutOffset.y), tile->width, static_cast<int>(mTileHeight)));

					mWalls.push_back(wall);
				}
				else if (tile->isGate)
				{
					Excluder gate;

					gate.Init(AARectangle(glm::vec2(startingX, layoutOffset.y), tile->width, static_cast<int>(mTileHeight)));

					mGate.push_back(gate);
				}
				
				if (tile->isPacmanSpawnTile)
				{
					mPacmanSpawnPosition = vec3(startingX + tile->offset.x, layoutOffset.y + tile->offset.y,0);
				}
				else if (tile->isRedGhostSpawnTile)
				{
					mRedGhostSpwanPosition = vec3(startingX + tile->offset.x + 1, layoutOffset.y + tile->offset.y,0);
				}
				else if (tile->isPinkGhostSpawnTile)
				{
					mPinkGhostSpwanPosition = vec3(startingX + tile->offset.x, layoutOffset.y + tile->offset.y, 0);
				}
				else if(tile->isBlueGhostSpwanTile)
				{
					mBlueGhostSpwanPosition = vec3(startingX + tile->offset.x + 1, layoutOffset.y + tile->offset.y, 0);
				}
				else if (tile->isCherrySpwanTile)
				{
					Fruit cherry;
					cherry.position = vec3(startingX + tile->offset.x, layoutOffset.y + tile->offset.y,0);
					cherry.mBBox = AARectangle(vec2(startingX, layoutOffset.y + tile->offset.y), PELLET_SIZE, PELLET_SIZE);
					cherry.name = FRUITE_NAME::CHERRY;
					cherry.score = CHERRY_SCORE;
					mFruits.push_back(cherry);
				}
				else if (tile->isAppleSpwanTile)
				{
					Fruit apple;
					apple.position = vec3(startingX + tile->offset.x, layoutOffset.y + tile->offset.y, 2);
					apple.mBBox = AARectangle(vec2(startingX, layoutOffset.y + tile->offset.y), PELLET_SIZE, PELLET_SIZE);
					apple.name = FRUITE_NAME::APPLE;
					apple.score = APPLE_SCORE;
					mFruits.push_back(apple);
				}

				if (tile->isExcludePelletTile)
				{
					mExclusionTiles.push_back(*tile);
				}

				startingX += tile->width;
			}
		}

		layoutOffset += glm::vec2(0, mTileHeight);
	};
	fileLoader.AddCommand(layoutCommand);

	return fileLoader.LoadFile(path);
}

PacmanLevel::Tile* PacmanLevel::GetTileBySymbol(char symbol)
{
	for (size_t i = 0; i < mTiles.size(); ++i)
	{
		if (mTiles[i].symbol == symbol)
		{
			return &mTiles[i];
		}
	}
	return nullptr;
}

bool PacmanLevel::WillCollide(const AARectangle& abbox, PacmanMovement direction) const 
{
	AARectangle bbox = abbox;
	bbox.MoveBy(GetMovementVector(direction));
	BoundaryEdge edge;

	for (const auto& wall : mWalls)
	{
		if (wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	for (const auto& gate : mGate)
	{
		if (gate.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;

}

bool PacmanLevel::WillCollide(const Ghost& ghost, GhostAI& ghostAI, PacmanMovement direction) const
{
	AARectangle abbox = ghost.GetBoundingBox();
	abbox.MoveBy(GetMovementVector(direction));
	BoundaryEdge edge;
	for (const auto& wall : mWalls)
	{
		if (wall.HasCollided(abbox, edge))
		{
			return true;
		}
	}

	for (const auto& gate : mGate)
	{
		if (!(ghostAI.IsGoBackHome() || ghostAI.GoingToLeaveHome()) && gate.HasCollided(abbox, edge))
		{
			return true;
		}
	}

	return false;
}

void PacmanLevel::ResetLevel()
{
	ResetBonusItems();
	ResetPellets();
	ResetSkyBox();
}

void PacmanLevel::ResetSkyBox()
{
	mSkybox = new Sprite("./assets/sky.png");
	mSkybox->transformation.scale = glm::vec2(WINDOWSIZE.x * 2.8, WINDOWSIZE.y * 2.5);
	mSkybox->transformation.position = glm::vec3(-(float)(WINDOWSIZE.x) + 150, -(float)(WINDOWSIZE.y * 1.2), -20);
}

void PacmanLevel::ResetBonusItems()
{
	mBonusTimer.resize(mFruits.size());
	for (auto f : mFruits)
	{
		f.eaten = false;
	}
}

void PacmanLevel::ResetPellets() 
{
	mPellets.clear();
	const uint32_t PADDING = static_cast<uint32_t>(mTileHeight);
	uint32_t startingY = (uint32_t)mLayoutOffset.y + PADDING + (uint32_t)mTileHeight - 20;
	uint32_t startingX = PADDING ;
	const uint32_t LEVEL_HEIGHT = (uint32_t)mLayoutOffset.y + 32 * (uint32_t)mTileHeight;
	Pellet p;
	p.score = PELLET_SCORE;
	uint32_t row = 0;

	for (uint32_t y = startingY; y < LEVEL_HEIGHT; y += PADDING, ++row)
	{
		for (uint32_t x = startingX, col = 0; x < WINDOWSIZE.x; x += PADDING, ++col)
		{
			if (row == 0 || row == 28)
			{
				if (col == 0 || col == 25)
				{
					p.powerPellet = 1;
					p.score = 40;
					p.mBBox = AARectangle(vec2(x - SUPER_PELLET_OFFSET, y), PELLET_SIZE, PELLET_SIZE + 5);
					mPellets.push_back(p);

					p.powerPellet = 0;
					p.score = 10;

					continue;
				}
			}

			AARectangle rect = AARectangle(vec2(x, y), PELLET_SIZE, PELLET_SIZE);
			bool found = false;
			for (const Excluder& wall : mWalls)
			{
				if (wall.GetAARectangle().Intersects(rect))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				for (const Tile& excludedPelletTile : mExclusionTiles)
				{
					if (excludedPelletTile.isExcludePelletTile)
					{
						AARectangle tileAABB(excludedPelletTile.position, excludedPelletTile.width, (uint32_t)mTileHeight);

						if (tileAABB.Intersects(rect))
						{
							found = true;
							break;
						}
					}
				}
			}

			if (!found)
			{
				p.mBBox = rect;
				mPellets.push_back(p);
			}
		}
	}
}

bool PacmanLevel::IsLevelOver() const
{
	uint32_t numEaten = 0;

	for (const auto& pellet : mPellets)
	{
		if (!pellet.powerPellet && pellet.eaten)
		{
			++numEaten;
		}
	}

	return numEaten >= mPellets.size() - 4;
}



