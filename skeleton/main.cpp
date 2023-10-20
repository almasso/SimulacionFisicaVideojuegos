

#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <deque>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include <unordered_map>

#include "Particle.h"
#include "Projectile.h"
#include "Plane.h"
#include "ParticleSystem.h"
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


std::deque<Projectile*> projectiles;
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

	//plane = new Plane(Vector3(0, 0, 0), Vector3(5000.0f,0.5f,5000.0f), Vector4(1,0,0,1));
	BoundingBox bb(Vector3(-50, 0, -50), Vector3(50, 600, 50));
	//partSystem = new ParticleSystem(bb, bb.bottomCenter(), { 0,50,0 }, { 5,10,5 });

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

	for (auto it = projectiles.begin(); it < projectiles.end();) {
		if (*it) { //Si el proyectil existe...
			(*it)->integrate(t);
			if ((*it)->getPosition().p.y < 0.0f || (*it)->getPosition().p.x > 750.0f || (*it)->getPosition().p.x < -750.0f || (*it)->getPosition().p.z > 750.0f || (*it)->getPosition().p.z < -750.0f ||
				(*it)->getStartTime() + 50 < GetLastTime()) {
				delete (*it);
				it = projectiles.erase(it);
			}
			else ++it;
		}
	}

}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	for (int i = 0; i < projectiles.size(); ++i) {
		auto e = projectiles.front();
		projectiles.pop_front();
		delete e;
	}

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
			projectiles.push_back(new Projectile(GetCamera()->getTransform().p, GetCamera()->getDir(), defaultProjectileSpeeds.at(projectile_mode), defaultProjectileSize.at(projectile_mode), projectile_mode, projectile_colors.at(projectile_mode)));
			break;
		}
		case 'B': {
			projectile_mode = projectile_mode == Projectile::ProjectileType::PROJECTILE_BULLET ? Projectile::ProjectileType::PROJECTILE_CANNONBALL : Projectile::ProjectileType::PROJECTILE_BULLET;
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