#pragma once

#include "Shape.h"

class Triangle: public Shape
{
public:
	Triangle();
	Triangle(const vec2& p0, const vec2& p1, const vec2& p2);

	inline void SetP0(const vec2& p0) {mPoints[0] = p0;}
	inline void SetP1(const vec2& p1) {mPoints[1] = p1;}
	inline void SetP2(const vec2& p2) {mPoints[2] = p2;}

	inline vec2 GetP0() const {return mPoints[0];}
	inline vec2 GetP1() const {return mPoints[1];}
	inline vec2 GetP2() const {return mPoints[2];}

	virtual vec2 GetCenterPoint() const override;

	float Area() const;

	bool ContainsPoint(const vec2& p) const;

private:
	float Area(const vec2& p0, const vec2& p1, const vec2& p2) const;
};

