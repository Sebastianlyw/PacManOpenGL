#include "Excluder.h"
#include "../Utilities/Utils.h"
#include <cmath>
#include <cassert>

void Excluder::Init(const AARectangle& rect, bool reverseNormals)
{
	mAARect = rect;
	mReverseNormals = reverseNormals;
	SetupEdges();
}

bool Excluder::HasCollided(const AARectangle& rect, BoundaryEdge& edge) const
{
	if (mAARect.Intersects(rect))
	{
		float yMin = mAARect.GetTopLeftPoint().y >= (rect.GetTopLeftPoint().y) ? mAARect.GetTopLeftPoint().y : rect.GetTopLeftPoint().y;
		float yMax = mAARect.GetBottomRightPoint().y <= rect.GetBottomRightPoint().y ? mAARect.GetBottomRightPoint().y : rect.GetBottomRightPoint().y;

		float ySize = yMax - yMin;

		float xMin = mAARect.GetTopLeftPoint().x >= rect.GetTopLeftPoint().x ? mAARect.GetTopLeftPoint().x : rect.GetTopLeftPoint().x;
		float xMax = mAARect.GetBottomRightPoint().x <= rect.GetBottomRightPoint().x ? mAARect.GetBottomRightPoint().x : rect.GetBottomRightPoint().x;

		float xSize = xMax - xMin;

		if (xSize > ySize)
		{
			if (rect.GetCenterPoint().y > mAARect.GetCenterPoint().y)
			{
				edge = mEdges[BOTTOM_EDGE];
			}
			else
			{
				edge = mEdges[TOP_EDGE];
			}
		}
		else
		{
			if (rect.GetCenterPoint().x < mAARect.GetCenterPoint().x)
			{
				edge = mEdges[LEFT_EDGE];
			}
			else
			{
				edge = mEdges[RIGHT_EDGE];
			}
		}

		return true;
	}

	return false;
}

glm::vec2 Excluder::GetCollisionOffset(const AARectangle& rect) const
{
	BoundaryEdge edge;
	glm::vec2 offset = glm::vec2(0.f);

	if (HasCollided(rect, edge))
	{
		float yMin = mAARect.GetTopLeftPoint().y >= (rect.GetTopLeftPoint().y) ? mAARect.GetTopLeftPoint().y : rect.GetTopLeftPoint().y;
		float yMax = mAARect.GetBottomRightPoint().y <= rect.GetBottomRightPoint().y ? mAARect.GetBottomRightPoint().y : rect.GetBottomRightPoint().y;

		float ySize = yMax - yMin;

		float xMin = mAARect.GetTopLeftPoint().x >= rect.GetTopLeftPoint().x ? mAARect.GetTopLeftPoint().x : rect.GetTopLeftPoint().x;
		float xMax = mAARect.GetBottomRightPoint().x <= rect.GetBottomRightPoint().x ? mAARect.GetBottomRightPoint().x : rect.GetBottomRightPoint().x;

		float xSize = xMax - xMin;

		if (!IsEqual(edge.normal.y, 0))
		{
			offset = (ySize + 1) * edge.normal;
		}
		else
		{
			offset = (xSize + 1) * edge.normal;
		}
	}

	return offset;
}

void Excluder::MoveBy(const glm::vec2& delta)
{
	mAARect.MoveBy(delta);
	SetupEdges();
}

void Excluder::MoveTo(const glm::vec2& point)
{
	mAARect.MoveTo(point);
	SetupEdges();
}

const BoundaryEdge& Excluder::GetEdge(EdgeType edge) const
{
	assert(edge != NUM_EDGES);
	return mEdges[edge];
}


void Excluder::SetupEdges()
{
	mEdges[TOP_EDGE].edge = { mAARect.GetTopLeftPoint().x, mAARect.GetTopLeftPoint().y, mAARect.GetBottomRightPoint().x, mAARect.GetTopLeftPoint().y };
	mEdges[TOP_EDGE].normal = UP_DIR;

	mEdges[LEFT_EDGE].edge = { mAARect.GetTopLeftPoint().x, mAARect.GetTopLeftPoint().y, mAARect.GetTopLeftPoint().x, mAARect.GetBottomRightPoint().y };
	mEdges[LEFT_EDGE].normal = LEFT_DIR;

	mEdges[BOTTOM_EDGE].edge = { mAARect.GetTopLeftPoint().x, mAARect.GetBottomRightPoint().y, mAARect.GetBottomRightPoint().x, mAARect.GetBottomRightPoint().y };
	mEdges[BOTTOM_EDGE].normal = DOWN_DIR;

	mEdges[RIGHT_EDGE].edge = { mAARect.GetBottomRightPoint().x, mAARect.GetTopLeftPoint().y, mAARect.GetBottomRightPoint().x, mAARect.GetBottomRightPoint().y };
	mEdges[RIGHT_EDGE].normal = RIGHT_DIR;

	if (mReverseNormals)
	{
		for (auto& edge : mEdges)
		{
			edge.normal = -edge.normal;
		}
	}
}
