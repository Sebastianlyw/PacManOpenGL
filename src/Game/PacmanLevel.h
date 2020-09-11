#pragma once

#include "../Math/Excluder.h"
#include <string>
#include <vector>
#include "gameHelper.h"
#include "GhostAI.h"
#include <random>
#include "../Graphics/Sprite.h"
#include <vector>

class Pacman;
class Ghost;
class GhostAI;
using namespace glm;


enum class FRUITE_NAME : unsigned char
{
	CHERRY = 0x01,
	APPLE  = 0x02
};

class PacmanLevel
{
public:
	PacmanLevel() :mAppleSprite(nullptr), mBackground(nullptr), mBlueGhostSpwanPosition(vec3(0)), mCherrySprite(nullptr),
		mLayoutOffset(vec2(0)), mPacmanSpawnPosition(vec3(0)), mPelletSprite(nullptr), mPinkGhostSpwanPosition(vec3(0)),
		mRedGhostSpwanPosition(vec3(0)), mPowerTimer(0), mSkybox(nullptr), mShaderTimer(0), mTileHeight(0) {}
	~PacmanLevel();
	bool Init(const std::string& levelPath);
	void Update(uint32_t dt, Pacman& pacman, std::vector<Ghost*>& ghosts);
	void Draw(uint32_t dt);
	void ResetLevel();
	void ResetSkyBox();
	void ResetBonusItems();
	bool WillCollide(const AARectangle& abbox, PacmanMovement direction) const;
	bool IsLevelOver() const;
	inline Sprite* GetBackground() { return mBackground; }
	inline Sprite* GetPelletSprite() { return mPelletSprite; }
	inline vec2 GetLayoutOffset() const { return mLayoutOffset; }
	inline vec3 GetPacmanSpawnPosition() { return mPacmanSpawnPosition; }
	inline vec3 GetRedghostSpwanPosition() { return mRedGhostSpwanPosition; }
	inline vec3 GetPinkghostSpwanPosition() { return mPinkGhostSpwanPosition; }
	inline vec3 GetBlueghostSpwanPosition() { return  mBlueGhostSpwanPosition; }
private:
	struct Tile
	{
		vec2 position = vec2(0.f);
		vec2 offset = vec2(0.f);
		int width = 0;
		bool isCollidable = false;
		bool isExcludePelletTile = false;
		bool isPacmanSpawnTile = false;
		bool isRedGhostSpawnTile = false;
		bool isBlueGhostSpwanTile = false;
		bool isPinkGhostSpawnTile = false;
		bool isCherrySpwanTile = false;
		bool isAppleSpwanTile = false;
		bool isGate = false;
		char symbol = '-';
	};

	struct Pellet
	{
		uint32_t score = 0;
		AARectangle mBBox;
		int powerPellet = 0;
		int eaten = 0;
	};


	struct Fruit
	{
		uint32_t score = APPLE_SCORE;
		AARectangle mBBox;
		int eaten = 0;
		FRUITE_NAME name = FRUITE_NAME::CHERRY;
		vec2 position = vec2(0.f);
	};

	bool LoadLevel(const std::string& path);
	Tile* GetTileBySymbol(char symbol);
	void ResetPellets();
	
	Sprite* mSkybox;
	Sprite* mBackground;
	Sprite* mPelletSprite;
	Sprite* mAppleSprite;
	Sprite* mCherrySprite;
	
	std::vector<Excluder> mWalls;
	std::vector<Excluder> mGate;
	std::vector<Tile> mTiles;
	std::vector<Tile> mExclusionTiles;
	std::vector<Pellet> mPellets;
	std::vector<Fruit> mFruits;

	size_t mTileHeight;
	vec2 mLayoutOffset;
	vec3 mPacmanSpawnPosition;
	vec3 mRedGhostSpwanPosition;
	vec3 mPinkGhostSpwanPosition;
	vec3 mBlueGhostSpwanPosition;
	float mShaderTimer;
	std::vector<double_t> mBonusTimer;
	uint32_t mPowerTimer;
};
