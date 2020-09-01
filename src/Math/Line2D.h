#pragma once

#include<glm/glm.hpp>
using namespace glm;

class Line2D
{
public:

	Line2D();
	Line2D(float x0, float y0, float x1, float y1);
	Line2D(const vec2& p0, const vec2& p1);

	inline const vec2& GetP0() const {return mP0;}
	inline const vec2& GetP1() const {return mP1;}

	inline void SetP0(const vec2& p0) {mP0 = p0;}
	inline void SetP1(const vec2& p1) {mP1 = p1;}

	bool operator==(const Line2D& line) const;

	float MinDistanceFrom(const vec2& p, bool limitToSegment = false) const;

	vec2 ClosestPoint(const vec2& p, bool limitToSegment = false) const;

	vec2 Midpoint() const;
	float Slope() const;
	float Length() const;

private:
	vec2 mP0;
	vec2 mP1;
};
