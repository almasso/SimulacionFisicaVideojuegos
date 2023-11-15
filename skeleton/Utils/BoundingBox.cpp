#include "BoundingBox.h"
#include "../checkMemoryLeaks.h"

bool BoundingBox::isInBoundingBox(const Vector3& pos) const {
	return ((pos.x >= _minimum.x && pos.x <= _maximum.x) && (pos.y >= _minimum.y && pos.y <= _maximum.y) && (pos.z >= _minimum.z && pos.z <= _maximum.z));
}

void BoundingBox::generateCorners() {
	Vector3 mxmz = _minimum;
	cornerTransforms[0] = physx::PxTransform(mxmz);
	corners[0] = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &cornerTransforms[0], Vector4(1, 1, 1, 1));

	Vector3 Mxmz = Vector3(_maximum.x, _minimum.y, _minimum.z);
	cornerTransforms[1] = physx::PxTransform(Mxmz);
	corners[1] = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &cornerTransforms[1], Vector4(1, 1, 1, 1));

	Vector3 mxMz = Vector3(_minimum.x, _minimum.y, _maximum.z);
	cornerTransforms[2] = physx::PxTransform(mxMz);
	corners[2] = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &cornerTransforms[2], Vector4(1, 1, 1, 1));

	Vector3 MxMz = Vector3(_maximum.x, _minimum.y, _maximum.z);
	cornerTransforms[3] = physx::PxTransform(MxMz);
	corners[3] = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &cornerTransforms[3], Vector4(1, 1, 1, 1));

	Vector3 mxMymz = Vector3(_minimum.x, _maximum.y, _minimum.z);
	cornerTransforms[4] = physx::PxTransform(mxMymz);
	corners[4] = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &cornerTransforms[4], Vector4(1, 1, 1, 1));

	Vector3 MxMymz = Vector3(_maximum.x, _maximum.y, _minimum.z);
	cornerTransforms[5] = physx::PxTransform(MxMymz);
	corners[5] = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &cornerTransforms[5], Vector4(1, 1, 1, 1));

	Vector3 mxMyMz = Vector3(_minimum.x, _maximum.y, _maximum.z);
	cornerTransforms[6] = physx::PxTransform(mxMyMz);
	corners[6] = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &cornerTransforms[6], Vector4(1, 1, 1, 1));

	Vector3 MxMyMz = _maximum;
	cornerTransforms[7] = physx::PxTransform(MxMyMz);
	corners[7] = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &cornerTransforms[7], Vector4(1, 1, 1, 1));
}

void BoundingBox::generateEdges() {
	//X != 1, Y = 1, Z = 1
	physx::PxBoxGeometry xNN(Vector3((_maximum.x + fabs(_minimum.x)) / 2, 1, 1));
	physx::PxShape* xNNS = CreateShape(xNN);
	//X = 1, Y != 1, Z = 1
	physx::PxBoxGeometry yNN(Vector3(1, (_maximum.y + fabs(_minimum.y)) / 2, 1));
	physx::PxShape* yNNS = CreateShape(yNN);
	//X = 1, Y = 1, Z != 1
	physx::PxBoxGeometry zNN(Vector3(1, 1, (_maximum.z + fabs(_minimum.z)) / 2));
	physx::PxShape* zNNS = CreateShape(zNN);

	Vector3 tmp = _minimum + Vector3((_maximum.x - _minimum.x) / 2, 0, 0);
	bLTransforms[0] = physx::PxTransform(tmp);
	boundingLines[0] = new RenderItem(xNNS, &bLTransforms[0], Vector4(1, 1, 1, 1)); //(minX, minY, minZ)

	tmp = _minimum + Vector3((_maximum.x - _minimum.x) / 2, 0, (_maximum.z - _minimum.z));
	bLTransforms[1] = physx::PxTransform(tmp);
	boundingLines[1] = new RenderItem(xNNS, &bLTransforms[1], Vector4(1, 1, 1, 1)); //(minX, minY, maxZ)

	tmp = _minimum + Vector3((_maximum.x - _minimum.x) / 2, (_maximum.y - _minimum.y), 0);
	bLTransforms[2] = physx::PxTransform(tmp);
	boundingLines[2] = new RenderItem(xNNS, &bLTransforms[2], Vector4(1, 1, 1, 1)); //(minX, maxY, minZ)

	tmp = _minimum + Vector3((_maximum.x - _minimum.x) / 2, (_maximum.y - _minimum.y), (_maximum.z - _minimum.z));
	bLTransforms[3] = physx::PxTransform(tmp);
	boundingLines[3] = new RenderItem(xNNS, &bLTransforms[3], Vector4(1, 1, 1, 1)); //(minX, maxY, maxZ)

	tmp = _minimum + Vector3(0, (_maximum.y - _minimum.y) / 2, 0);
	bLTransforms[4] = physx::PxTransform(tmp);
	boundingLines[4] = new RenderItem(yNNS, &bLTransforms[4], Vector4(1, 1, 1, 1)); //(minX, minY, minZ)

	tmp = _minimum + Vector3(_maximum.x - _minimum.x, (_maximum.y - _minimum.y) / 2, 0);
	bLTransforms[5] = physx::PxTransform(tmp);
	boundingLines[5] = new RenderItem(yNNS, &bLTransforms[5], Vector4(1, 1, 1, 1)); //(maxX, minY, minZ)

	tmp = _minimum + Vector3(_maximum.x - _minimum.x, (_maximum.y - _minimum.y) / 2, _maximum.z - _minimum.z);
	bLTransforms[6] = physx::PxTransform(tmp);
	boundingLines[6] = new RenderItem(yNNS, &bLTransforms[6], Vector4(1, 1, 1, 1)); //(maxX, minY, maxZ)

	tmp = _minimum + Vector3(0, (_maximum.y - _minimum.y) / 2, _maximum.z - _minimum.z);
	bLTransforms[7] = physx::PxTransform(tmp);
	boundingLines[7] = new RenderItem(yNNS, &bLTransforms[7], Vector4(1, 1, 1, 1)); //(minX, minY, maxZ)

	tmp = _minimum + Vector3(0, 0, (_maximum.z - _minimum.z) / 2);
	bLTransforms[8] = physx::PxTransform(tmp);
	boundingLines[8] = new RenderItem(zNNS, &bLTransforms[8], Vector4(1, 1, 1, 1)); //(minX, minY, minZ)

	tmp = _minimum + Vector3((_maximum.x - _minimum.x), 0, (_maximum.z - _minimum.z) / 2);
	bLTransforms[9] = physx::PxTransform(tmp);
	boundingLines[9] = new RenderItem(zNNS, &bLTransforms[9], Vector4(1, 1, 1, 1)); //(maxX, minY, minZ)

	tmp = _minimum + Vector3(0, (_maximum.y - _minimum.y), (_maximum.z - _minimum.z) / 2);
	bLTransforms[10] = physx::PxTransform(tmp);
	boundingLines[10] = new RenderItem(zNNS, &bLTransforms[10], Vector4(1, 1, 1, 1)); //(minX, maxY, minZ)

	tmp = _minimum + Vector3((_maximum.x - _minimum.x), (_maximum.y - _minimum.y), (_maximum.z - _minimum.z) / 2);
	bLTransforms[11] = physx::PxTransform(tmp);
	boundingLines[11] = new RenderItem(zNNS, &bLTransforms[11], Vector4(1, 1, 1, 1)); //(maxX, maxY, minZ)
}
