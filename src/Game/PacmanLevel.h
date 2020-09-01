#pragma once

#include "../Math/Excluder.h"
#include <string>
#include <vector>
#include "gameHelper.h"
#include "GhostAI.h"
#include "PacmanNew.h"
#include <random>
#include "../Graphics/Sprite.h"
#include "../Graphics/Camera.h"

using namespace glm;
class PacmanLevel
{
public:

	PacmanLevel();
	bool Init(const std::string& levelPath);
	void Update(float dt, PacmanNew& pacman);
	void Draw(float dt);
	void ResetLevel();

	bool WillCollide(const AARectangle& abbox, PacmanMovement direction) const;
	inline Sprite* GetBackground() { return mBackground; }
	inline Sprite* GetPelletSprite() { return mPelletSprite; }
	inline vec2 GetLayoutOffset() const { return mLayoutOffset; }
	inline vec2 GetPacmanSpawnPosition() { return mPacmanSpawnPosition; }


private:
	struct Tile
	{
		vec2 position = vec2(0.f);
		vec2 offset = vec2(0.f);
		int width = 0;
		bool collidable = false;
		bool excludePelletTile = false;
		bool pacmanSpawnTile = false;
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
	vec2 mPacmanSpawnPosition;
};
