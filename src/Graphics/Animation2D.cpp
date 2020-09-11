#include "Animation2D.h"

#include <string>
#include <sstream>
#include <glad/glad.h>

Animation2D::Animation2D(const char* filename, bool looped)
	: mAnimCursor(0),
	mCurrentFrameIndex(0),
	mSpeed(50),
	mIsPlaying(true),
	mLooped(looped),
	mFinisedAnimation(false)
{
	FILE* fp = nullptr;
	const int bufferlen = 255;
	char line[bufferlen];

	fopen_s(&fp, filename, "r");

	if (fp == nullptr)
	{
		printf("erorr in reading animation file \n");
	}
	else
	{
		while (fgets(line, bufferlen, fp))
		{
			vector<int> result;

			stringstream ss(line);
			string token;
			while (getline(ss, token, ','))
			{
				result.push_back(stoi(token));
			}

			glm::vec4 frame = glm::vec4(result[0], result[1], result[2], result[3]);
			frames.push_back(frame);
		}
	}

	mFrameCount = (int)frames.size();

	fclose(fp);
}

Animation2D::~Animation2D()
{
}

void Animation2D::Pause()
{
	mIsPlaying = !mIsPlaying;
}

void Animation2D::play(Texture& spritetexture, Rectangle& rectangle, uint32_t deltatime)
{
	if (mIsPlaying && !mFinisedAnimation)
	{
		mAnimCursor += deltatime;

		//Looped animation
		if (mAnimCursor > mSpeed)
		{   //0,1,2  count 3
			if ((mCurrentFrameIndex + 1) == mFrameCount && !mLooped)
			{
				mFinisedAnimation = true;
			}
			else
			{
				mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameCount;
				mAnimCursor = (mAnimCursor - mSpeed); //reset and take over the remainder to animation timer accumulator.
			}
		}

		glm::vec4 frame = frames[mCurrentFrameIndex];

		// normalization
		frame.x /= spritetexture.Getwidth();
		frame.y /= spritetexture.Getheight();
		frame.z /= spritetexture.Getwidth();
		frame.w /= spritetexture.Getheight();

		vector<glm::vec2> uv;

		uv = {
			glm::vec2(frame.x,frame.y),
			glm::vec2(frame.x, frame.y + frame.w),
			glm::vec2(frame.x + frame.z, frame.y),

			glm::vec2(frame.x + frame.z, frame.y),
			glm::vec2(frame.x, frame.y + frame.w),
			glm::vec2(frame.x + frame.z, frame.y + frame.w)
		};

		glBindVertexArray(rectangle.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, rectangle.UVBO);

		/*{ //realocation for memory
			glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), &uv[0], GL_DYNAMIC_DRAW);
		}*/


		// best practice to send data to gpu memory..
		void* gpubuffer = nullptr;
		gpubuffer = glMapBufferRange(GL_ARRAY_BUFFER, 0, 6 * sizeof(glm::vec2), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		memcpy(gpubuffer, uv.data(), 6 * sizeof(glm::vec2));
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Animation2D::SetAnimationSpeed(uint32_t newspeed)
{
	mSpeed = newspeed;
}

