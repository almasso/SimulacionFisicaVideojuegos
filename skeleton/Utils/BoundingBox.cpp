#include "BoundingBox.h"
#include "../checkMemoryLeaks.h"

bool BoundingBox::isInBoundingBox(const Vector3& pos) const {
	return ((pos.x >= _minimum.x && pos.x <= _maximum.x) && (pos.y >= _minimum.y && pos.y <= _maximum.y) && (pos.z >= _minimum.z && pos.z <= _maximum.z));
}
