#pragma once

#include "Shape.h"

using namespace glm;

class AARectangle: public Shape
{
public:
	AARectangle();
	AARectangle(const vec2& topLeft, unsigned int width, unsigned int height);
	AARectangle(const vec2& topLeft, const vec2& bottomRight);

	inline void SetTopLeftPoint(const vec2& topLeft) {mPoints[0] = topLeft;}
	inline void SetBottomRightPoint(const vec2& bottomRight) {mPoints[1] = bottomRight;}

	vec2 GetTopLeftPoint() const {return mPoints[0];}
	vec2 GetBottomRightPoint() const {return mPoints[1];}

	float GetWidth() const;
	float GetHeight() const;

	void MoveTo(const vec2& position);

	virtual vec2 GetCenterPoint() const override;

	bool Intersects(const AARectangle& otherRect) const;
	bool ContainsPoint(const vec2& point) const;

	static AARectangle Inset(const AARectangle& rect,const vec2& insets);

	virtual std::vector<vec2> GetPoints() const override;
};

