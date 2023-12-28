#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"
#include <vector>
#include <iostream>

using Point = Vector3;
class BoundingBox {
private:
	Point _minimum;
	Point _maximum;
	std::vector<RenderItem*> boundingLines;
	std::vector<RenderItem*> corners;
	std::vector<physx::PxTransform> cornerTransforms;
	std::vector<physx::PxTransform> bLTransforms;
	void generateCorners();
	void generateEdges();
	bool _showing;

public:
	BoundingBox() : BoundingBox(Point(0,0,0), Point(0,0,0)) {}
	BoundingBox(const Point& minimum, const Point& maximum, bool showBB = false) : _minimum(minimum), _maximum(maximum), boundingLines(12,nullptr), corners(8, nullptr), cornerTransforms(8), bLTransforms(12), _showing(showBB) {
		if (showBB) {
			show();
		}
	}

	~BoundingBox() {
		hide();
	}

	inline Point maximum() const { return _maximum; }
	inline Point minimum() const { return _minimum; }

	inline Point center() const { return ((_maximum + _minimum) / 2); }
	inline Point bottomCenter() const { return Point(center().x, minimum().y, center().z); }
	inline Point topCenter() const { return Point(center().x, maximum().y, center().z); }
	inline Vector3 dimensions() const { return _maximum - _minimum; }

	bool isInBoundingBox(const Vector3& pos) const;

	bool isShowing() const { return _showing; }

	inline void show() { 
		generateCorners();
		generateEdges();
		_showing = true;
	}

	inline void hide() {
		for (auto e : corners) {
			if (e) e->release();
		}

		for (auto b : boundingLines) {
			if (b) b->release();
		}
		_showing = false;
	}
};

class BoundingSphere {
private:
	Point _center;
	float _radius;
public:
	BoundingSphere(const Point& center, float radius) : _center(center), _radius(radius) {}

	inline Point center() const { return _center; }
	inline float radius() const { return _radius; }
	
	inline bool isInBoundingSphere(const Vector3& pos) const { return (pos - _center).magnitude() <= _radius; }

};

