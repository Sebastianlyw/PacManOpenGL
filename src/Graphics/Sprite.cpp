#include "Sprite.h"
#include "..//Math/AARectangle.h"

Sprite::Sprite(const char* filename)
{
	texture = new Texture(filename);
	rectangle = new Rectangle();
}


Sprite::~Sprite()
{
	delete texture;
	delete rectangle;

	for (auto& o : mAnimations)
		delete o;
}


void Sprite::Init(const char* filename)
{
	texture = new Texture(filename);
	rectangle = new Rectangle();
}

void Sprite::Update(double dt)
{

}

void Sprite::Stop()
{
	for (auto& o : mAnimations)
	{
		o->Pause();
	}
}



void Sprite::draw(double deltatime, AnimationType type)
{
	if (!mAnimations.empty() && mAnimations[type])
	{
		if(deltatime > 0)
		{
			mAnimations[type]->play(*texture, *rectangle, deltatime);
		}
		else
		{
			mAnimations[type]->PlayOneFrame();
			mAnimations[type]->play(*texture, *rectangle, deltatime);
		}
	}

	texture->Bind();
	rectangle->Draw();
}

void Sprite::draw_wireframe()
{
	rectangle->Draw_Wireframe();
}


void Sprite::AddAnimation(const char* filename, bool looped)
{
	auto anim = new Animation2D(filename, looped);

	mAnimations.push_back(anim);
}

const AARectangle Sprite::GetBoundingBox() const
{
	AARectangle bbox = { Position(), static_cast<uint32_t>(Size().x), static_cast<uint32_t>(Size().y) };
	return bbox;
}