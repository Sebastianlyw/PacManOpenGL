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


namespace {
	const float SUPER_PELLET_OFFSET = 12;
}
PacmanLevel::PacmanLevel()
{
	ResourceManager::LoadShader("./shaders/level.vs", "./shaders/level.fs", nullptr, "level");
}

PacmanLevel::~PacmanLevel()
{
	delete mBackground;
	delete mSkybox;
	delete mPelletSprite;
	delete mCherry.sprite;
	delete mApple.sprite;
}

bool PacmanLevel::Init(const std::string& levelPath) 
{
	AudioPlayer::instance().Play(AudioPlayer::BACKGROUND, true);
	
	bool levelLoaded = LoadLevel(levelPath);
	if (levelLoaded)
	{
		ResetLevel();
	}
	return levelLoaded;
}


void PacmanLevel::Update(float dt, Pacman& pacman, Ghost& redGhost) 
{
	/*mSkybox->transformation.position.y -= dt * 20;*/
	//Collision checking game logic here. 
	for (const auto & wall : mWalls)
	{
		BoundaryEdge edge;

		if (wall.HasCollided(pacman.GetBoundingBox(), edge))
		{
			vec2 offset = wall.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		if (wall.HasCollided(redGhost.GetBoundingBox(), edge))
		{
			vec2 offset = wall.GetCollisionOffset(redGhost.GetBoundingBox());
			redGhost.MoveBy(offset);
			redGhost.Stop();
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
					if (!redGhost.IsDead())
					{
						redGhost.SetToVulnerable();
					}
				}
			}
		}
	}

	if (pacman.GetEatingBoundingBox().Intersects(mCherry.mBBox) && !mCherry.eaten)
	{
		mCherry.eaten = true;
		pacman.AteItem(mCherry.score);
		AudioPlayer::instance().Play(AudioPlayer::EAT_FRUIT, false);
	}

	if (pacman.GetEatingBoundingBox().Intersects(mApple.mBBox) && !mApple.eaten)
	{
		mApple.eaten = true;
		pacman.AteItem(mApple.score);
		pacman.SetSpeedUp(true);
		pacman.SetMovementSpeed(PACMAN_SPEED_UP);
		AudioPlayer::instance().Play(AudioPlayer::EAT_FRUIT, false);
	}

}

void PacmanLevel::Draw(float dt)
{
	ShaderManager shader = ResourceManager::GetShader("level");

	// pass projection matrix to shader (note that in this case it could change every frame)
	shader.Use().SetMatrix4("projection", Camera::instance().GetPerspectiveProjection());
	shader.SetMatrix4("view", Camera::instance().GetViewMatrix());
	
	shader.SetInteger("isSkyBox", 1);
	mGameTimer += dt/80;
	if (mGameTimer >= 1)
	{
		mGameTimer = 0;
	}
	shader.SetFloat("deltaTime", mGameTimer);
	shader.SetMatrix4("model_matrx", mSkybox->transformation.Get());
	mSkybox->draw(0);
	shader.SetFloat("isSkyBox", 0 );
	shader.SetMatrix4("model_matrx", mBackground->transformation.Get());
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

	if (!mCherry.eaten)
	{
		shader.SetMatrix4("model_matrx", mCherry.sprite->transformation.Get());
		mCherry.sprite->draw(0);
	}


	if (!mApple.eaten)
	{
		shader.SetMatrix4("model_matrx", mApple.sprite->transformation.Get());
		mApple.sprite->draw(0);
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
		mBackground->transformation.position = glm::vec3(0, 40,0);
		mSkybox = new Sprite("./assets/space3-2.png");
		mSkybox->transformation.scale = glm::vec2(WINDOWSIZE.x*3, WINDOWSIZE.y*3);
		mSkybox->transformation.position = glm::vec3(-(float)(WINDOWSIZE.x),-(float)(WINDOWSIZE.y*1.2), -20);
		assert(mBackground->IsLoaded() && "Didn't load the bg image");
		assert(mSkybox->IsLoaded() && "Didn't load the bg image");
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
		mCherry.sprite = new Sprite(("./" + std::string("assets/") + imageName).c_str());
		mCherry.sprite->transformation.scale = PACMAN_SIZE;
		assert(mCherry.sprite->IsLoaded() && "Didn't load the cherry image");
	};
	fileLoader.AddCommand(cherryImageCommand);

	Command appleImageCommand;
	appleImageCommand.command = "apple_image";
	appleImageCommand.parseFunc = [this, &imageName](ParseFuncParams params)
	{
		imageName = FileCommandLoader::ReadString(params);
		mApple.sprite = new Sprite(("./" + std::string("assets/") + imageName).c_str());
		mApple.sprite->transformation.scale = PACMAN_SIZE;
		assert(mApple.sprite->IsLoaded() && "Didn't load the cherry image");
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
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params)
	{
		int startingX = layoutOffset.x;

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
				if (tile->isPacmanSpawnTile)
				{
					mPacmanSpawnPosition = vec3(startingX + tile->offset.x, layoutOffset.y + tile->offset.y,2);
				}
				if (tile->isRedGhostSpawnTile)
				{
					mRedGhostSpwanPosition = vec3(startingX + tile->offset.x, layoutOffset.y + tile->offset.y,2);
				}
				if (tile->isCherrySpwanTile)
				{
					mCherry.sprite->SetPosition(vec3(startingX + tile->offset.x, layoutOffset.y + tile->offset.y,2));
					AARectangle rect = AARectangle(vec2(startingX, layoutOffset.y + tile->offset.y), PELLET_SIZE, PELLET_SIZE);
					mCherry.mBBox = rect;
				}
				if (tile->isAppleSpwanTile)
				{
					mApple.sprite->SetPosition(vec3(startingX + tile->offset.x, layoutOffset.y + tile->offset.y, 2));
					AARectangle rect = AARectangle(vec2(startingX, layoutOffset.y + tile->offset.y), PELLET_SIZE, PELLET_SIZE);
					mApple.mBBox = rect;
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

	return false;

}

void PacmanLevel::ResetLevel()
{
	ResetPellets();
}

void PacmanLevel::ResetPellets() 
{
	mPellets.clear();
	const uint32_t PADDING = static_cast<uint32_t>(mTileHeight);
	uint32_t startingY = mLayoutOffset.y + PADDING + mTileHeight - 20;
	uint32_t startingX = PADDING;
	const uint32_t LEVEL_HEIGHT = mLayoutOffset.y + 32 * mTileHeight;
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
					p.mBBox = AARectangle(vec2(x - SUPER_PELLET_OFFSET, y), PELLET_SIZE, PELLET_SIZE);
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
						AARectangle tileAABB(excludedPelletTile.position, excludedPelletTile.width, mTileHeight);

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



