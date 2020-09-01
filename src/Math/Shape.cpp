#include "Shape.h"

void Shape::MoveBy(const vec2& deltaOffset)
{
	for(vec2& point : mPoints)
	{
		point = point + deltaOffset;
	}
}
