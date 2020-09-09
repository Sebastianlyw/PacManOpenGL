#include <cmath>
#include "Line2D.h"
#include "../Utilities/Utils.h"

Line2D::Line2D():Line2D(vec2(0.f), vec2(0.f))
{

}

Line2D::Line2D(float x0, float y0, float x1, float y1): Line2D(vec2(x0, y0), vec2(x1, y1))
{

}

Line2D::Line2D(const vec2& p0, const vec2& p1):mP0(p0), mP1(p1)
{

}

bool Line2D::operator==(const Line2D& line) const
{
	return line.GetP0() == mP0 && line.GetP1() == mP1;
}

float Line2D::Slope() const
{
	float dx = mP1.x - mP0.x;

	if(fabsf(dx) < EPSILON)
	{
		return 0;
	}

	float dy = mP1.y - mP0.y;

	return dy / dx;
}

vec2 Line2D::Midpoint() const
{
	return vec2((mP0.x + mP1.x)/ 2.0f, (mP0.y + mP1.y)/2.0f);
}

float Line2D::MinDistanceFrom(const vec2& p, bool limitToSegment) const
{
	
	return distance(p, ClosestPoint(p, limitToSegment));
}

vec2 Line2D::ClosestPoint(const vec2& p, bool limitToSegment) const
{
	vec2 p0ToP = p - mP0;

	vec2 p0ToP1 = mP1 - mP0;

	constexpr float l2 = (float)(p0ToP1.length() * p0ToP1.length());

	float dot =   glm::dot(p0ToP,p0ToP1);

	float t = dot / l2;

	if(limitToSegment)
	{
		t = (float)std::fmax(0, std::fmin(1.0f, t));
	}

	return mP0 + p0ToP1 * t;
}


float Line2D::Length() const
{
	return distance(mP1, mP0);
}
