#include "Particle.h"
#include "../checkMemoryLeaks.h"
#include <iostream>
//#define DEBUG

Particle::Particle(Particle& p) {
	this->data.type = p.data.type;
	this->data.vel = p.data.vel;
	this->data.damping = p.data.damping;
	this->data.pose = p.data.pose;
	this->data.lifeTime = 0.0;
	this->data.size = p.data.size;
	this->data.colour = p.data.colour;
	this->data.force = p.data.force;
	this->data.inv_mass = p.data.inv_mass;
	
	if (!this->data.renderItem) {
		physx::PxSphereGeometry sphere(this->data.size);
		physx::PxShape* shape = CreateShape(sphere);
		this->data.renderItem = new RenderItem(shape, &(this->data.pose), this->data.colour);
	}
}

Particle::Particle(Particle_Type type, float damping, Vector4 Col) {
	data.type = type;
	data.damping = damping;
	data.colour = Col;
}

Particle::Particle(Particle_Type type, float inv_mass, Vector3 pos, Vector3 vel, float damping, Vector4 Col) : Particle(type, inv_mass, pos, vel, 1, damping, Col) {}

Particle::Particle(Particle_Type type, float inv_mass, Vector3 pos, Vector3 vel, float size, float damping, Vector4 Col) {
	data.type = type;
	data.vel = vel;
	data.pose = physx::PxTransform(pos);
	data.damping = damping;
	data.size = size;
	data.colour = Col;
	physx::PxSphereGeometry sphere(this->data.size);
	physx::PxShape* shape = CreateShape(sphere);
	data.renderItem = new RenderItem(shape, &data.pose, Col);
	data.lifeTime = 0.0f;
	data.force = Vector3(0, 0, 0);
	data.inv_mass = inv_mass;
}

Particle::~Particle() {
	if(data.renderItem != nullptr) data.renderItem->release();
}

void Particle::integrate(double t) {
	//Euler semi-implícito
	Vector3 resultAccel = data.force * data.inv_mass;
	data.vel += resultAccel * t;
	data.vel *= powf(data.damping, t);
	data.pose.p += data.vel * t;

	data.lifeTime += t;

	clearForce();

#ifdef DEBUG
	std::cout << "POSICIÓN:(" << pose.p.x << "," << pose.p.y << "," << pose.p.z << ")\n";
	std::cout << "VELOCIDAD:(" << vel.x << "," << vel.y << "," << vel.z << ")\n";
	std::cout << "ACELERACIÓN:(" << acceleration.x << "," << acceleration.y << "," << acceleration.z << ")\n";
#endif
}

SolidParticle::SolidParticle(physx::PxPhysics* gPhysics, physx::PxScene* gScene, SolidParticle& p) : Particle(p) {
	esfera = (this->data.inv_mass <= 0.0f) ? static_cast<physx::PxRigidActor*>(gPhysics->createRigidStatic(this->data.pose)) : static_cast<physx::PxRigidActor*>(gPhysics->createRigidDynamic(this->data.pose));
	physx::PxSphereGeometry sphere(this->data.size);
	physx::PxShape* shape = CreateShape(sphere);
	this->data.inv_mass <= 0.0f ? static_cast<physx::PxRigidStatic*>(esfera)->attachShape(*shape) : static_cast<physx::PxRigidDynamic*>(esfera)->attachShape(*shape);
	if (this->data.inv_mass > 0.0f) {
		physx::PxRigidBodyExt::updateMassAndInertia(*(static_cast<physx::PxRigidDynamic*>(esfera)), getParticleVolume() * this->data.inv_mass);
		static_cast<physx::PxRigidDynamic*>(esfera)->setLinearVelocity(this->data.vel);
	}
	gScene->addActor(*esfera);
	this->data.renderItem = new RenderItem(shape, esfera, this->data.colour);
}

SolidParticle::SolidParticle(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Particle_Type type, float damping, Vector4 Col) : Particle(type, damping, Col) {}

SolidParticle::SolidParticle(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Particle_Type type, float inv_mass, Vector3 Pos, Vector3 Vel, float damping, Vector4 Col) : SolidParticle(gPhysics, gScene, type, inv_mass, Pos, Vel, 1, damping, Col) {}

SolidParticle::SolidParticle(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Particle_Type type, float inv_mass, Vector3 Pos, Vector3 Vel, float size, float damping, Vector4 Col) : Particle(type, inv_mass, Pos, Vel, size, damping, Col) {
	this->data.renderItem->release();
	esfera = (this->data.inv_mass <= 0.0f) ? static_cast<physx::PxRigidActor*>(gPhysics->createRigidStatic(this->data.pose)) : static_cast<physx::PxRigidActor*>(gPhysics->createRigidDynamic(this->data.pose));
	physx::PxSphereGeometry sphere(this->data.size);
	physx::PxShape* shape = CreateShape(sphere);
	this->data.inv_mass <= 0.0f ? static_cast<physx::PxRigidStatic*>(esfera)->attachShape(*shape) : static_cast<physx::PxRigidDynamic*>(esfera)->attachShape(*shape);
	if (this->data.inv_mass > 0.0f) {
		physx::PxRigidBodyExt::updateMassAndInertia(*(static_cast<physx::PxRigidDynamic*>(esfera)), getParticleVolume() * this->data.inv_mass);
		static_cast<physx::PxRigidDynamic*>(esfera)->setLinearVelocity(this->data.vel);
	}
	gScene->addActor(*esfera);
	this->data.renderItem = new RenderItem(shape, esfera, this->data.colour);
}

void SolidParticle::integrate(double t) {

}