#pragma once
#include "Texture.h"
#include "Rectangle.h"
#include "Transform.h"
#include "Animation2D.h"
#include <stdint.h>
#include <string>

class AARectangle;

//Animated sprite. 
class Sprite
{
public:
	Sprite(const char* filename);
	~Sprite();

	Transform transformation;
	
	void Init(const char* filename);
	void Update(double dt);
	void draw(double deltatime, AnimationType type);
	void draw_wireframe();
	void AddAnimation(const char* filename, bool looped);
	void Stop();
	const AARectangle GetBoundingBox() const;

	inline glm::vec2 Size() const { return transformation.scale; }
	inline bool IsLoaded() const { return (texture->Getdata() != NULL); }
	inline glm::vec2 Position() const { return transformation.position; }
	inline void SetPosition(const glm::vec2& position) { transformation.position = position; }
	inline void MoveBy(const glm::vec2& delta) { transformation.position += delta; }
	inline bool IsFinishedPlayingAnimation() const { return false; } //return mAnimationPlayer.IsFinishedPlaying(); }
	
private:
	Texture* texture;
	Rectangle* rectangle;

	vector<Animation2D*> mAnimations;
	glm::vec3 mColor;
};

