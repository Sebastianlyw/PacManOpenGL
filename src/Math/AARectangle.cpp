#include <cmath>
#include "AARectangle.h"
#include "../Utilities/Utils.h"

AARectangle::AARectangle(): AARectangle(vec2(0.f), vec2(0.f))
{

}

AARectangle::AARectangle(const vec2& topLeft, uint32_t width, uint32_t height)
{
	mPoints.push_back(topLeft);
	mPoints.push_back(vec2(topLeft.x + width - 1, topLeft.y + height-1));
}

AARectangle::AARectangle(const vec2& topLeft, const vec2& bottomRight)
{
	mPoints.push_back(topLeft);
	mPoints.push_back(bottomRight);
}

float AARectangle::GetWidth() const
{
	return GetBottomRightPoint().x - GetTopLeftPoint().x + 1;
}

float AARectangle::GetHeight() const
{
	return GetBottomRightPoint().y - GetTopLeftPoint().y + 1;
}

void AARectangle::MoveTo(const vec2& position)
{
	float width = GetWidth();
	float height = GetHeight();

	SetTopLeftPoint(position);
	SetBottomRightPoint(vec2(position.x + width - 1, position.y + height - 1));
}

vec2 AARectangle::GetCenterPoint() const
{
	return vec2(GetTopLeftPoint().x + GetWidth()/2.0f, GetTopLeftPoint().y + GetHeight()/2.0f);
}

bool AARectangle::Intersects(const AARectangle& otherRect) const
{
	return !(otherRect.GetBottomRightPoint().x < GetTopLeftPoint().x ||
			otherRect.GetTopLeftPoint().x > GetBottomRightPoint().x ||
			otherRect.GetBottomRightPoint().y < GetTopLeftPoint().y ||
			otherRect.GetTopLeftPoint().y > GetBottomRightPoint().y);
}

bool AARectangle::ContainsPoint(const vec2& point) const
{
	bool withinX = point.x >= GetTopLeftPoint().x && point.x <= GetBottomRightPoint().x;
	bool withinY = point.y >= GetTopLeftPoint().y && point.y <= GetBottomRightPoint().y;

	return withinX && withinY;
}

AARectangle AARectangle::Inset(const AARectangle& rect,const vec2& insets)
{
	return AARectangle(rect.GetTopLeftPoint() + insets, (uint32_t)(rect.GetWidth() - 2*insets.x), (uint32_t)(rect.GetHeight() - 2*insets.y));
}

std::vector<vec2> AARectangle::GetPoints() const
{
	std::vector<vec2> points;

	points.push_back(mPoints[0]);
	points.push_back(vec2(mPoints[1].x, mPoints[0].y));
	points.push_back(mPoints[1]);
	points.push_back(vec2(mPoints[0].x, mPoints[1].y));

	return points;
}
