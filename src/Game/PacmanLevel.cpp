#include "PacmanLevel.h"
#include "..//Utilities/FileCommandLoader.h"
#include "PacmanNew.h"
#include "..//Graphics/shaderManager.h"
#include "..//Utilities/resourceManager.h"
#include "gameHelper.h"

bool PacmanLevel::Init(const std::string& levelPath) 
{
	bool levelLoaded = LoadLevel(levelPath);
	if (levelLoaded)
	{
		ResetLevel();
	}
	return levelLoaded;
}


void PacmanLevel::Update(float dt, PacmanNew& pacman) 
{
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
	}

	for (auto& pellet : mPellets)
	{
		if (!pellet.eaten)
		{
			if (pacman.GetEatingBoundingBox().Intersects(pellet.mBBox))
			{
				pellet.eaten = true;

				pacman.AteItem(pellet.score);

			/*	if (pellet.powerPellet)
				{
					pacman.ResetGhostEatenMultiplier();
					for (auto& ghost : ghosts)
					{
						ghost.SetStateToVulnerable();
					}
				}*/
			}
		}
	}
}

void PacmanLevel::Draw(float dt)
{
	//Drawa pellets:
	ShaderManager shader = ResourceManager::GetShader("sprite");
	shader.Use().SetMatrix4("model_matrx", mBackground->transformation.Get());
	mBackground->draw(0, AnimationType::Idle);

	for (const auto& pellet : mPellets)
	{
		if (!pellet.eaten)
		{
			mPelletSprite->transformation.position = pellet.mBBox.GetCenterPoint();
			shader.SetMatrix4("model_matrx", mPelletSprite->transformation.Get());
			mPelletSprite->draw(0, AnimationType::Idle);
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
		mBackground->transformation.scale = glm::vec2(WINDOWSIZE.x, WINDOWSIZE.y);
		assert(mBackground->IsLoaded() && "Didn't load the bg image");
	};
	fileLoader.AddCommand(bgImageCommand);

	Command pelletImageCommand;
	pelletImageCommand.command = "pellet_image";
	pelletImageCommand.parseFunc = [this, &imageName](ParseFuncParams params)
	{
		imageName = FileCommandLoader::ReadString(params);
		mPelletSprite = new Sprite(("./" + std::string("assets/") + imageName).c_str());
		mPelletSprite->transformation.scale = glm::vec2(20, 20);
		assert(mPelletSprite->IsLoaded() && "Didn't load the bg image");
	};
	fileLoader.AddCommand(pelletImageCommand);

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
		mTiles.back().collidable = FileCommandLoader::ReadInt(params);
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
		mTiles.back().excludePelletTile = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileExcludePelletCommand);

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

				if (tile->collidable > 0)
				{
					Excluder wall;
					wall.Init(AARectangle(glm::vec2(startingX, layoutOffset.y), tile->width, static_cast<int>(mTileHeight)));

					mWalls.push_back(wall);
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
	const uint32_t PELLET_SIZE = 45;
	const uint32_t PADDING = static_cast<uint32_t>(mTileHeight);
	uint32_t startingY = mLayoutOffset.y + PADDING + mTileHeight - 30;
	uint32_t startingX = PADDING;
	const uint32_t LEVEL_HEIGHT = mLayoutOffset.y + 32 * mTileHeight;
	Pellet p;
	p.score = 10;
	uint32_t row = 0;

	for (uint32_t y = startingY; y < LEVEL_HEIGHT; y += PADDING, ++row)
	{
		for (uint32_t x = startingX, col = 0; x < WINDOWSIZE.x; x += PADDING, ++col)
		{
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
					if (excludedPelletTile.excludePelletTile)
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



