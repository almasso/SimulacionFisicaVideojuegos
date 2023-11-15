#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"
#include <vector>

using Point = Vector3;
class BoundingBox {
private:
	Point _minimum;
	Point _maximum;
	RenderItem* boundingLine;

public:
	BoundingBox() : BoundingBox(Point(0,0,0), Point(0,0,0)) {}
	BoundingBox(const Point& minimum, const Point& maximum, bool showBB = false) : _minimum(minimum), _maximum(maximum), boundingLine(nullptr) {
		if(showBB) show();
	}

	~BoundingBox() {
		hide();
	}

	inline Point maximum() const { return _maximum; }
	inline Point minimum() const { return _minimum; }

	inline Point center() const { return dimensions() / 2; }
	inline Point bottomCenter() const { return Point(center().x, minimum().y, center().z); }
	inline Point topCenter() const { return Point(center().x, maximum().y, center().z); }
	inline Vector3 dimensions() const { return (_maximum - _minimum); }

	bool isInBoundingBox(const Vector3& pos) const;

	inline void show() { 
		physx::PxBoxGeometry cube(_maximum - _minimum);
		physx::PxShape* shape = CreateShape(cube);
		boundingLine = new RenderItem(shape, &physx::PxTransform(_minimum), Vector4(1,1,1,0));
		////X != 1, Y = 1, Z = 1
		//physx::PxBoxGeometry xNN(Vector3(_maximum.x - _minimum.x, 10, 10));
		//physx::PxShape* xNNS = CreateShape(xNN);
		////X = 1, Y != 1, Z = 1
		//physx::PxBoxGeometry yNN(Vector3(10, _maximum.y - _minimum.y, 10));
		//physx::PxShape* yNNS = CreateShape(yNN);
		////X = 1, Y = 1, Z != 1
		//physx::PxBoxGeometry zNN(Vector3(10, 10, _maximum.z - _minimum.z));
		//physx::PxShape* zNNS = CreateShape(zNN);

		//boundingLines[0] = new RenderItem(xNNS, &physx::PxTransform(_minimum), Vector4(1, 1, 1, 1)); //(minX, minY, minZ)
		//boundingLines[1] = new RenderItem(xNNS, &physx::PxTransform(Vector3(_minimum.x, _minimum.y, _maximum.z)), Vector4(1, 1, 1, 1)); //(minX, minY, maxZ)
		//boundingLines[2] = new RenderItem(xNNS, &physx::PxTransform(Vector3(_minimum.x, _maximum.y, _minimum.z)), Vector4(1, 1, 1, 1)); //(minX, maxY, minZ)
		//boundingLines[3] = new RenderItem(xNNS, &physx::PxTransform(Vector3(_minimum.x, _maximum.y, _maximum.z)), Vector4(1, 1, 1, 1)); //(minX, maxY, maxZ)

		//boundingLines[4] = new RenderItem(yNNS, &physx::PxTransform(_minimum), Vector4(1, 1, 1, 1)); //(minX, minY, minZ)
		//boundingLines[5] = new RenderItem(yNNS, &physx::PxTransform(Vector3(_maximum.x, _minimum.y, _minimum.z)), Vector4(1, 1, 1, 1)); //(maxX, minY, minZ)
		//boundingLines[6] = new RenderItem(yNNS, &physx::PxTransform(Vector3(_maximum.x, _minimum.y, _maximum.z)), Vector4(1, 1, 1, 1)); //(maxX, minY, maxZ)
		//boundingLines[7] = new RenderItem(yNNS, &physx::PxTransform(Vector3(_minimum.x, _minimum.y, _maximum.z)), Vector4(1, 1, 1, 1)); //(minX, minY, maxZ)

		//boundingLines[8] = new RenderItem(zNNS, &physx::PxTransform(_minimum), Vector4(1, 1, 1, 1)); //(minX, minY, minZ)
		//boundingLines[9] = new RenderItem(zNNS, &physx::PxTransform(Vector3(_maximum.x, _minimum.y, _minimum.z)), Vector4(1, 1, 1, 1)); //(maxX, minY, minZ)
		//boundingLines[10] = new RenderItem(zNNS, &physx::PxTransform(Vector3(_minimum.x, _maximum.y, _minimum.z)), Vector4(1, 1, 1, 1)); //(minX, maxY, minZ)
		//boundingLines[11] = new RenderItem(zNNS, &physx::PxTransform(Vector3(_maximum.x, _maximum.y, _minimum.z)), Vector4(1, 1, 1, 1)); //(maxX, maxY, minZ)
	}
	void hide() { 
		 boundingLine->release();
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

