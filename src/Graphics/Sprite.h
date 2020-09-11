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
	void Update(uint32_t dt);
	void draw(uint32_t deltatime, AnimationType type = AnimationType::Walking);
	void draw_wireframe();
	void AddAnimation(const char* filename, bool looped);
	void Stop();
	const AARectangle GetBoundingBox() const;

	inline glm::vec2 Size() const { return transformation.scale; }
	inline bool IsLoaded() const { return (texture->Getdata() != NULL); }
	inline glm::vec3 Position() const { return transformation.position; }
	inline void SetPosition(const glm::vec3& position) { transformation.position = position; }
	inline void SetSize(const glm::vec2& size) { transformation.scale = size; }
	inline void SetRotation(const float rotation) { transformation.rotation = rotation; }
	inline void MoveBy(const glm::vec2& delta) { transformation.position += glm::vec3(delta,0); }
	inline bool IsFinishedPlayingAnimation() const { return false; } //return mAnimationPlayer.IsFinishedPlaying(); }
	
private:
	Texture* texture;
	Rectangle* rectangle;

	vector<Animation2D*> mAnimations;
	glm::vec3 mColor;
};

