#include "Circle.h"
#include "../Utilities/Utils.h"

Circle::Circle(): Circle(vec2(0,0), 0)
{

}

Circle::Circle(const vec2& center, float radius):mRadius(radius)
{
	mPoints.push_back(center);
}

bool Circle::Intersects(const Circle& otherCircle) const
{
	
	return distance(GetCenterPoint(), otherCircle.GetCenterPoint()) < (mRadius + otherCircle.mRadius);
}

bool Circle::ContainsPoint(const vec2& point) const
{
	
	return IsLessThanOrEqual(distance(GetCenterPoint(), point), mRadius);
}
