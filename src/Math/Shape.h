#pragma once

#include<glm/glm.hpp>
#include <vector>

using namespace glm;

class Shape
{
public:
	virtual vec2 GetCenterPoint() const = 0;
	virtual ~Shape() {}
	inline virtual std::vector<vec2> GetPoints() const {return mPoints;}
	void MoveBy(const vec2& deltaOffset);
protected:
	std::vector<vec2> mPoints;
};

