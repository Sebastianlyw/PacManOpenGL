#pragma once
#include "Texture.h"
#include "Rectangle.h"
#include <glm/glm.hpp>
#include <vector>
using namespace std;


enum AnimationType
{
	Walking,
	Death
};

class Animation2D
{
public:
	Animation2D(const char* filename, bool looped = false);

	~Animation2D();


	void play(Texture& spritetexture, Rectangle& rectangle, uint32_t deltatime);

	void SetAnimationSpeed(uint32_t newspeed);

	void Pause();
	inline void PlayOneFrame() { mCurrentFrameIndex = 3;  }

private:
	uint32_t mAnimCursor;
	int mCurrentFrameIndex;
	int mFrameCount;
	uint32_t mSpeed;
	bool mIsPlaying;
	bool mLooped;
	bool mFinisedAnimation;
	vector<glm::vec4> frames;
};
