#pragma once

#include "AARectangle.h"
#include "BoundaryEdge.h"

class Excluder
{
public:
	virtual ~Excluder() {}
	void Init(const AARectangle& rect, bool reverseNormals = false);
	bool HasCollided(const AARectangle& rect, BoundaryEdge& edge) const;
	glm::vec2 GetCollisionOffset(const AARectangle& rect) const;
	inline const AARectangle& GetAARectangle() const { return mAARect; }
	void MoveBy(const glm::vec2& delta);
	void MoveTo(const glm::vec2& point);
	const BoundaryEdge& GetEdge(EdgeType edge) const;

private:
	void SetupEdges();
	AARectangle mAARect;
	BoundaryEdge mEdges[NUM_EDGES];
	bool mReverseNormals;
};
