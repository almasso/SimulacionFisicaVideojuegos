#pragma once
#include "../RenderUtils.hpp"
#include <cmath>
#include "../Particle/Particle.h"
#include "../Extras/Plane.h"
namespace angleutils {
	static float degToRad(float deg) {
		return (deg * physx::PxPi) / 180.0;
	}
	static float radToDeg(float rad) {
		return (rad * 180.0) / physx::PxPi;
	}
	static void rotateRigidActor(SolidPlane* plane, const Vector3& pivot, const physx::PxQuat& rotation) {
		Vector3 newPosition = rotation.rotate(plane->getPos().p - pivot) + pivot;
		physx::PxQuat newOrientation = rotation * plane->getPos().q;
		plane->setPose(physx::PxTransform(newPosition, newOrientation));
	}
	static void rotateRigidActor(SolidParticle* particle, const Vector3& pivot, const physx::PxQuat& rotation) {
		Vector3 newPosition = rotation.rotate(particle->getPosition().p - pivot) + pivot;
		physx::PxQuat newOrientation = rotation * particle->getPosition().q;
		particle->setPosition(physx::PxTransform(newPosition, newOrientation));
	}
}