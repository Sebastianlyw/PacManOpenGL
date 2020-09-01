#pragma once

#include "Shape.h"

using namespace glm;

class Circle: public Shape
{
public:

	Circle();
	Circle(const vec2& center, float radius);

	inline virtual vec2 GetCenterPoint() const override {return mPoints[0];}
	inline float GetRadius() const {return mRadius;}
	inline void SetRadius(float radius) {mRadius = radius;}
	inline void MoveTo(const vec2& position) {mPoints[0] = position;}

	bool Intersects(const Circle& otherCircle) const;
	bool ContainsPoint(const vec2& point) const;

private:
	float mRadius;
};

