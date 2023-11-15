

#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <deque>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include <unordered_map>

#include "Particle/Particle.h"
#include "Particle/Projectile.h"
#include "Extras/Plane.h"
#include "Systems/ParticleSystem.h"
#include "Systems/ProjectileSystem.h"
#include "Generators/ParticleGenerator.h"
#include "Generators/ForceGenerator.h"
#include "checkMemoryLeaks.h"

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

std::string bullet_text = "Bullet Mode";
Projectile::ProjectileType projectile_mode = Projectile::ProjectileType::PROJECTILE_BULLET;
std::unordered_map<Projectile::ProjectileType, Vector4> projectile_colors = {
	{Projectile::ProjectileType::PROJECTILE_BULLET, Vector4(1,1,0,1)},
	{Projectile::ProjectileType::PROJECTILE_CANNONBALL, Vector4(0,1,1,1)}
};
std::unordered_map<Projectile::ProjectileType, float> defaultProjectileSpeeds = {
	{Projectile::ProjectileType::PROJECTILE_BULLET, 75.0f},
	{Projectile::ProjectileType::PROJECTILE_CANNONBALL, 100.0f}
};
std::unordered_map<Projectile::ProjectileType, float> defaultProjectileSize = {
	{Projectile::ProjectileType::PROJECTILE_BULLET, 0.6f},
	{Projectile::ProjectileType::PROJECTILE_CANNONBALL, 4.0f}
};
Plane* plane;
ParticleSystem* partSystem;
ProjectileSystem* projSystem;
BoundingBox* bb = new BoundingBox(Vector3(-50, 0, -50), Vector3(50, 600, 50), true);
BoundingBox* bb2 = new BoundingBox(Vector3(-750, 0, -750), Vector3(750, 2000, 750));


#ifdef PARTICLE
Particle* particle;
#endif




// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	GetCamera()->getTransform().rotate(Vector3(0, 0, 0));

	plane = new Plane(Vector3(0, 0, 0), Vector3(5000.0f,0.5f,5000.0f), Vector4(1,0,0,1));
	bb2->hide();
	partSystem = new ParticleSystem(*bb);
	projSystem = new ProjectileSystem(*bb2);
	//partSystem->addParticleGenerator(new GaussianParticleGenerator(Particle::Particle_Type::NORMAL, "mainGaussianParticleGenerator", bb.bottomCenter(), Vector3(0.01f, 0.001f, 0.01f), Vector3(0, 50, 0.0f), Vector3(15, 20, 15), 5.0f, 5.0f));
	partSystem->addParticleGenerator(new UniformParticleGenerator(Particle::Particle_Type::NORMAL, "mainUniformParticleGenerator", bb->bottomCenter(), Vector3(10, 1, 10), Vector3(1.0f, 50, 1.0f), Vector3(5, 10, 5), 5.0f, 5.0f));
	//partSystem->addForceGenerator(new GravityForceGenerator(Vector3(0, -9.8, 0)));
	partSystem->addForceGenerator(new ParticleDragGenerator(Vector3(-100 ,0, 70), 0.5f, 0.0f));
	projSystem->addForceGenerator(new GravityForceGenerator(Vector3(0, -9.8, 0)));

#ifdef PARTICLE
	particle = new Particle(GetCamera()->getTransform().p, GetCamera()->getDir() * 20);
#endif
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	partSystem->update(t);
	projSystem->update(t);
	switch (projectile_mode) {
		case Projectile::ProjectileType::PROJECTILE_BULLET: bullet_text = "Bullet Mode"; break;
		case Projectile::ProjectileType::PROJECTILE_CANNONBALL: bullet_text = "Cannonball Mode"; break;
	}

#ifdef PARTICLE
	particle->integrate(t);
#endif

	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	delete plane;
	delete partSystem;
	delete projSystem;
	delete bb2;
	delete bb;

#ifdef PARTICLE
	delete particle;
#endif
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key)) {
		case ' ':
		{
			projSystem->generateProjectile(new Projectile(GetCamera()->getTransform().p, GetCamera()->getDir(), defaultProjectileSpeeds.at(projectile_mode), defaultProjectileSize.at(projectile_mode), projectile_mode, projectile_colors.at(projectile_mode)));
			break;
		}
		case 'B': {
			projectile_mode = projectile_mode == Projectile::ProjectileType::PROJECTILE_BULLET ? Projectile::ProjectileType::PROJECTILE_CANNONBALL : Projectile::ProjectileType::PROJECTILE_BULLET;
			break;
		}
		case 'F': {
			partSystem->generateFirework(Vector3(0, 0, 0), Vector3(0, 50, 0), 4, 0.998f, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1));
			break;
		}
		default:
			break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}