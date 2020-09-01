#pragma once

#include "../Math/Excluder.h"
#include <string>
#include <vector>
#include "gameHelper.h"
#include "GhostAI.h"
#include <random>
#include "../Graphics/Sprite.h"

class PacmanNew;
class Ghost;

using namespace glm;
class PacmanLevel
{
public:

	bool Init(const std::string& levelPath);
	void Update(float dt, PacmanNew& pacman);
	void Draw(float dt);
	void ResetLevel();

	bool WillCollide(const AARectangle& abbox, PacmanMovement direction) const;
	inline Sprite* GetBackground() { return mBackground; }
	inline Sprite* GetPelletSprite() { return mPelletSprite; }
	inline vec2 GetLayoutOffset() const { return mLayoutOffset; }


private:
	struct Tile
	{
		vec2 position = vec2(0.f);
		vec2 offset = vec2(0.f);
		int width = 0;
		int collidable = 0;
		int excludePelletTile = 0;
		int pacmanSpawnPoint = 0;
		char symbol = '-';
	};

	struct Pellet
	{
		uint32_t score = 0;
		AARectangle mBBox;
		int powerPellet = 0;
		int eaten = 0;
	};

	Sprite* mBackground;
	Sprite* mPelletSprite;
	bool LoadLevel(const std::string& path);	
	Tile* GetTileBySymbol(char symbol);
	void ResetPellets();
	
	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;
	std::vector<Tile> mExclusionTiles;
	std::vector<Pellet> mPellets;

	size_t mTileHeight;
	vec2 mLayoutOffset;
};
