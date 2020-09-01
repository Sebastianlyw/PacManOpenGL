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


	void play(Texture& spritetexture, Rectangle& rectangle, double deltatime);

	void SetAnimationSpeed(float newspeed);

	void Pause();
	inline void PlayOneFrame() { mCurrentFrameIndex = 3;  }

private:
	double mAnimCursor;
	int mCurrentFrameIndex;
	int mFrameCount;
	float mSpeed;
	bool mIsPlaying;
	bool mLooped;
	bool mFinisedAnimation;
	vector<glm::vec4> frames;
};
