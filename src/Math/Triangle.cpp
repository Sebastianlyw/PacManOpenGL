#include <cmath>
#include "Triangle.h"
#include "../Utilities/Utils.h"

Triangle::Triangle():Triangle(vec2(0.f), vec2(0.f), vec2(0.f))
{

}

Triangle::Triangle(const vec2& p0, const vec2& p1, const vec2& p2)
{
	mPoints.push_back(p0);
	mPoints.push_back(p1);
	mPoints.push_back(p2);
}

vec2 Triangle::GetCenterPoint() const
{
	return vec2((mPoints[0].x + mPoints[1].x + mPoints[2].x) /3.0f, (mPoints[0].y + mPoints[1].y + mPoints[2].y) /3.0f);
}

float Triangle::Area() const
{
	return Area(GetP0(), GetP1(), GetP2());
}

bool Triangle::ContainsPoint(const vec2& p) const
{
	float thisArea = Area();

	float a1 = Area(p, GetP1(), GetP2());

	float a2 = Area(GetP0(), p, GetP2());

	float a3 = Area(GetP0(), GetP1(), p);

	return IsEqual(thisArea, a1 + a2 + a3);
}

float Triangle::Area(const vec2& p0, const vec2& p1, const vec2& p2) const
{
	return fabsf((p0.x * (p1.y - p2.y) + p1.x*(p2.y - p0.y) + p2.x*(p0.y - p1.y))/2.0f);
}
