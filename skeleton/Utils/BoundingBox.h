#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"

using Point = Vector3;
class BoundingBox {
private:
	Point _minimum;
	Point _maximum;
public:
	BoundingBox() : _minimum(Point(0, 0, 0)), _maximum(Point(0, 0, 0)) {}
	BoundingBox(const Point& minimum, const Point& maximum) : _minimum(minimum), _maximum(maximum) {}

	inline Point maximum() const { return _maximum; }
	inline Point minimum() const { return _minimum; }

	inline Point center() const { return dimensions() / 2; }
	inline Point bottomCenter() const { return Point(center().x, minimum().y, center().z); }
	inline Point topCenter() const { return Point(center().x, maximum().y, center().z); }
	inline Vector3 dimensions() const { return (_maximum - _minimum); }

	bool isInBoundingBox(const Vector3& pos) const;
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

