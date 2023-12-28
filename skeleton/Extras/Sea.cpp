#include "Sea.h"
#include <iostream>

Sea::Sea(BoundingBox* bb, Vector4 col) : pose(physx::PxTransform(bb->center())) {
	std::cout << "X: " << bb->dimensions().x << " Y: " << bb->dimensions().y << " Z: " << bb->dimensions().z << "\n";
	physx::PxBoxGeometry planeGeo(bb->dimensions() / 2);
	physx::PxShape* shape = CreateShape(planeGeo);
	renderItem = new RenderItem(shape, &pose, col);
}

Sea::~Sea() {
	renderItem->release();
}
