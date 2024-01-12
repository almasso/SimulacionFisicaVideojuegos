

#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <deque>
#include <algorithm>

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
#include "Registries/BoundingBoxRegistry.h"
#include "Messages/Message.h"
#include "Text/Text.h"
#include "Project/Field.h"
#include "Project/Martillo.h"
#include "checkMemoryLeaks.h"
#include "Utils/ColorUtils.h"
#include "Utils/AngleUtils.h"
#define PROJECT

//std::string display_text = "This is a test";
//std::string bullet_text = "Bullet Mode";
TextManager textManager(100, nullptr);


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

#ifndef PROJECT
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

SolidPlane* plane;
ParticleSystem* partSystem;
ProjectileSystem* projSystem;
BoundingBoxRegistry* bbReg;
bool gG = false, dG = false, wG = false;
GravityForceGenerator* currGG = nullptr;
ParticleDragGenerator* currDG = nullptr;
WhirlpoolGenerator* currWG = nullptr;
bool pause = false;

#ifdef PARTICLE
Particle* particle;
#endif
#endif //PROJECT
#ifdef PROJECT
BoundingBoxRegistry* _bbReg;
ParticleSystem* _mainPS, *_hammerHit;
ParticleDragGenerator* wind;
Field* field;
Martillo* martillo;
float martilloRot = 0.5f;
float rotationChangeMartillo = 0.005f;
bool startRotating = false, startCounting = false, finalRonda = false, finJuego = false;
int tirada = 1, totalRondas = 5, viento = 0;
float tiempoEntreRondas = 5.0f, tiempoMaxEntreRondas = 5.0f;
float maximaPuntuacion = 0.0f;
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

#ifndef PROJECT
	//plane = new Plane(Vector3(0, 0, 0), Vector3(100.0f, 0.1f, 100.0f), Vector4(1, 0, 0, 1));
	plane = new SolidPlane(gPhysics, gScene, Vector3(0, 0, 0), Vector3(5000.0f,0.1f,5000.0f), Vector4(1,0,0,1));
	bbReg = new BoundingBoxRegistry();
	bbReg->addRegistry("particleSysBB", new BoundingBox(Vector3(-600, -150, -600), Vector3(600, 300, 600)));
	bbReg->addRegistry("projectileBB", new BoundingBox(Vector3(-750, 0, -750), Vector3(750, 2000, 750)));
	bbReg->addRegistry("dragAppBB", new BoundingBox(Vector3(-250, 0, -250), Vector3(250, 150, 250)));
	bbReg->addRegistry("whirlpoolAppBB", new BoundingBox(Vector3(-150, 0, -150), Vector3(150, 150, 150)));
	partSystem = new ParticleSystem(*(bbReg->at("particleSysBB")));
	projSystem = new ProjectileSystem(*(bbReg->at("projectileBB")));
	//GaussianSolidParticleGenerator* gspg = new GaussianSolidParticleGenerator(gPhysics, gScene, Particle::Particle_Type::NORMAL, "mainGaussianParticleGenerator", (bbReg->at("particleSysBB"))->center(), Vector3(0.01f, 0.001f, 0.01f), Vector3(0, 50, 0.0f), Vector3(15, 20, 15), 5.0f, 5.0f);
	//gspg->setMaximumParticles(1000);
	//partSystem->addParticleGenerator(gspg);
	//UniformSolidParticleGenerator* uspg = new UniformSolidParticleGenerator(gPhysics, gScene, Particle::Particle_Type::SOLID, Particle::Particle_Shape::MIX, "mainUniformParticleGenerator", (bbReg->at("particleSysBB"))->bottomCenter() + Vector3(0,150,0), Vector3(10, 1, 10), Vector3(1.0f, 50, 1.0f), Vector3(5, 10, 5), 5.0f, 5.0f);
	//uspg->setMaximumParticles(1000);
	//partSystem->addParticleGenerator(uspg);

	//partSystem->addParticleGenerator(new GaussianParticleGenerator(Particle::Particle_Type::NORMAL, "mainGaussianParticleGenerator", (bbReg->at("particleSysBB"))->center(), Vector3(0.01f, 0.001f, 0.01f), Vector3(0, 50, 0.0f), Vector3(15, 20, 15), 5.0f, 5.0f));
	//partSystem->addParticleGenerator(new UniformParticleGenerator(Particle::Particle_Type::NORMAL, "mainUniformParticleGenerator",(bbReg->at("particleSysBB"))->bottomCenter(), Vector3(10, 1, 10), Vector3(1.0f, 50, 1.0f), Vector3(5, 10, 5), 5.0f, 5.0f));
	
	//projSystem->addForceGenerator(new GravityForceGenerator(Vector3(0, -9.8, 0)));

#ifdef PARTICLE
	particle = new Particle(GetCamera()->getTransform().p, GetCamera()->getDir() * 20);
#endif
#endif //PROJECT
#ifdef PROJECT
	_bbReg = new BoundingBoxRegistry();
	_bbReg->addRegistry("mainBB", new BoundingBox(Vector3(-1000, -1000, -1000), Vector3(1000, 1000, 1000)));
	_bbReg->addRegistry("generationBB", new BoundingBox(Vector3(-150, -150, -150), Vector3(150, 150, 150)));
	_bbReg->addRegistry("windBB", new BoundingBox(Vector3(-600, -150, -400), Vector3(700, 150, 400)));
	_mainPS = new ParticleSystem(*_bbReg->at("mainBB"));
	field = new Field(gPhysics, gScene, _bbReg->at("generationBB")->bottomCenter());
	martillo = new Martillo(gPhysics, gScene, _mainPS, field->getFieldSouthmostPos() + Vector3(45, 18, 0));
	GetCamera()->setTransform(field->getFieldSouthmostPos() + Vector3(40, 20, 0));
	GetCamera()->setDir(Vector3(1, -0.02, 0.033));
	
	textManager[0] = new Text("Velocidad de giro del martillo: 0", Vector2(90, 450), colorutils::hexToVec4(0xff0000));
	textManager[1] = new Text("Distancias obtenidas", Vector2(320, 300), colorutils::hexToVec4(0x85ded6));
	textManager[2] = new Text("1 - 0.00m", Vector2(320, 280), colorutils::hexToVec4(0x85ded6));
	textManager[3] = new Text("2 - 0.00m", Vector2(320, 260), colorutils::hexToVec4(0x85ded6));
	textManager[4] = new Text("3 - 0.00m", Vector2(320, 240), colorutils::hexToVec4(0x85ded6));
	textManager[5] = new Text("4 - 0.00m", Vector2(320, 220), colorutils::hexToVec4(0x85ded6));
	textManager[6] = new Text("5 - 0.00m", Vector2(320, 200), colorutils::hexToVec4(0x85ded6));
	textManager[7] = new Text("Siguiente ronda en 5", Vector2(150, 350), colorutils::hexToVec4(0x00ffff));
	textManager[7]->setShow(false);
	textManager[8] = new Text("FIN DE LA PARTIDA", Vector2(160, 350), colorutils::hexToVec4(0xff00f7));
	textManager[8]->setShow(false);
	textManager[9] = new Text("Hay viento de cara! Debes lanzar con mas potencia!", Vector2(35, 10), colorutils::hexToVec4(0x05ffea));
	textManager[9]->setShow(false);
	textManager[10] = new Text("MAXIMA PUNTUACION: 0.00m", Vector2(120, 320), colorutils::hexToVec4(0xff00f7));
	textManager[10]->setShow(false);
	textManager[11] = new Text("ESC para salir, R para reiniciar", Vector2(120, 280), colorutils::hexToVec4(0x05deff));
	textManager[11]->setShow(false);
#endif
}

void finPartida() {
	textManager[8]->setShow(true);
	textManager[9]->setShow(false);
	textManager[10]->setText("MAXIMA PUNTUACION: " + std::to_string(maximaPuntuacion) + "m");
	textManager[10]->setShow(true);
	textManager[11]->setShow(true);
	for (int i = 0; i < 8; ++i) {
		textManager[i]->setShow(false);
	}
	finJuego = true;
}


void resetRonda() {
	if (tirada != 0) {
		delete _hammerHit;
		_hammerHit = nullptr;
		BoundingBox* tmp = _bbReg->at("hitBB");
		_bbReg->deleteRegister("hitBB");
		delete tmp;
	}

	GetCamera()->setTransform(field->getFieldSouthmostPos() + Vector3(40, 20, 0));
	GetCamera()->setDir(Vector3(1, -0.02, 0.033));
	delete martillo;
	martillo = new Martillo(gPhysics, gScene, _mainPS, field->getFieldSouthmostPos() + Vector3(45, 18, 0));
	tiempoEntreRondas = tiempoMaxEntreRondas;
	finalRonda = false;
	textManager[7]->setShow(false);
	martilloRot = 0.5f;
	rotationChangeMartillo = 0.005f;
	textManager[0]->setText("Velocidad de giro del martillo: 0");
	
	if (tirada < totalRondas) {
		tirada++;
		viento = rand() % 2;
		std::cout << viento << "\n";
	}
	else {
		finPartida();
		return;
	}

	if (wind) {
		message::Message m((int)message::msgID::_m_GENERATOR_ERASABLE);
		m.genData.fg = wind;
		message::MessageManager::sendMessage(m);
		wind = nullptr;
	}
	textManager[9]->setShow(false);

	if(viento) textManager[9]->setShow(true);
}

void resetPartida() {
	textManager[8]->setShow(false);
	textManager[10]->setShow(false);
	textManager[11]->setShow(false);
	for (int i = 0; i < 8; ++i) {
		textManager[i]->setShow(true);
		if (i > 1 && i < 7) {
			textManager[i]->setText(std::to_string(i - 1) + " - 0.00m");
			textManager[i]->setColor(colorutils::hexToVec4(0x85ded6));
		}
	}
	tirada = 0;
	finJuego = false;
	maximaPuntuacion = 0.0f;
	resetRonda();
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
#ifndef PROJECT
	if (!pause) {
		partSystem->update(t);
		projSystem->update(t);
		switch (projectile_mode) {
		case Projectile::ProjectileType::PROJECTILE_BULLET: bullet_text = "Bullet Mode"; break;
		case Projectile::ProjectileType::PROJECTILE_CANNONBALL: bullet_text = "Cannonball Mode"; break;
		}
	}
	else {
		std::string command;
		int value;
		std::cout << "Introduce un comando: ";
		std::cin >> command;
		if (command == "changeK") {
			std::cin >> value;
			message::Message m((int)message::msgID::_m_CHANGE_HOOKE_CONSTANT);
			m.hookeValue.val = value;
			message::MessageManager::sendMessage(m);
		}
		pause = false;
	}

#ifdef PARTICLE
	particle->integrate(t);
#endif
#endif //PROJECT

#ifdef PROJECT
	_mainPS->update(t);
	if (startRotating) {
		martillo->move(martilloRot);
		martilloRot += rotationChangeMartillo;
		rotationChangeMartillo += 0.000001f;
		textManager[0]->setText("Velocidad de giro del martillo: " + std::to_string(martillo->getVel()));
	}
	if (startCounting) {
		if (viento) {
			if (!wind) {
				wind = new ParticleDragGenerator((_bbReg->at("windBB")), Vector3(-300, 0, 0), 1.5f, 0.0f);
				_mainPS->addForceGenerator(wind);
			}
		}
		textManager[tirada + 1]->setText(std::to_string(tirada) + " - " + std::to_string(martillo->getDistance()) + "m");
		GetCamera()->setTransform(martillo->getBallPos() + Vector3(-10, 90, 0));
		GetCamera()->setDir(Vector3(1, -3, 0));
	}
	if (finalRonda) {
		_hammerHit->update(t);
		GetCamera()->setTransform(martillo->getBallPos() + Vector3(-10, 90, 0));
		GetCamera()->setDir(Vector3(1, -3, 0));
		textManager[7]->setShow(true);
		if (tiempoEntreRondas <= 0.0f) {
			resetRonda();
		}
		else {
			if (tiempoEntreRondas > 4.6f && tiempoEntreRondas < 4.8f) _hammerHit->setGenerate(false);
			tiempoEntreRondas -= t;
			textManager[7]->setText("Siguiente ronda en " + std::to_string((int)(tiempoEntreRondas)));
		}
	}
#endif

	gScene->simulate(t);
	gScene->fetchResults(true);

}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
#ifndef PROJECT
	delete plane;
	delete partSystem;
	delete projSystem;
	
	for (auto it = bbReg->begin(); it != bbReg->end();) {
		BoundingBox* tmp = it->second;
		it = bbReg->erase(it);
		delete tmp;
	}
	delete bbReg;

#ifdef PARTICLE
	delete particle;
#endif
#endif // PROJECT
#ifdef PROJECT
	delete field;
	delete martillo;
	for (auto it = _bbReg->begin(); it != _bbReg->end();) {
		BoundingBox* tmp = it->second;
		it = _bbReg->erase(it);
		delete tmp;
	}
	for (auto t : textManager) delete t;
	delete _bbReg;
	delete _mainPS;
	if(_hammerHit) delete _hammerHit;
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
#ifdef PROJECT
	if (!finJuego) {
		switch (toupper(key)) {
			/*case 'M': {
				for (auto it = _bbReg->begin(); it != _bbReg->end(); ++it) {
					it->second->isShowing() ? it->second->hide() : it->second->show();
				}
				break;
			}*/
			case 'R': {
				startRotating = true;
			} break;
			case ' ': {
				if (startRotating) {
					martillo->lanzar();
					startRotating = false;
					startCounting = true;
				}
			} break;
		}
	}
	else {
		switch (toupper(key)) {
			case 'R': resetPartida(); break;
		}
	}
#endif

#ifndef PROJECT
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
		case 'E': {
			partSystem->generateExplosion((bbReg->at("particleSysBB")));
			break;
		}
		case 'F': {
			partSystem->generateFirework(Vector3(0, 0, 0), Vector3(0, 50, 0), 4, 0.998f, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1));
			break;
		}
		case '7': {
			partSystem->generateSpringDemo();
			break;
		}
		case '8': {
			partSystem->generateAnchoredSpringDemo();
			break;
		}
		case '9': {
			partSystem->generateBuoyancyDemo();
			break;
		}
		case 'C': {
			pause = true;
			break;
		}
		case 'M': {
			for (auto it = bbReg->begin(); it != bbReg->end(); ++it) {
				it->second->isShowing() ? it->second->hide() : it->second->show();
			}
			break;
		}
		case '1':
		{
			if (!gG) {
				gG = true;
				currGG = new GravityForceGenerator(Vector3(0, -9.8, 0));
				partSystem->addForceGenerator(currGG);
			}
			else {
				message::Message m((int)message::msgID::_m_GENERATOR_ERASABLE);
				m.genData.fg = currGG;
				message::MessageManager::sendMessage(m);
				gG = false;
			}
			break;
		}
		case '2':
		{
			if (!dG) {
				dG = true;
				currDG = new ParticleDragGenerator((bbReg->at("dragAppBB")), Vector3(-100, 0, 70), 0.5f, 0.0f);
				partSystem->addForceGenerator(currDG);
			}
			else {
				message::Message m((int)message::msgID::_m_GENERATOR_ERASABLE);
				m.genData.fg = currDG;
				message::MessageManager::sendMessage(m);
				dG = false;
			}
			break;
		}
		case '3':
		{
			if (!wG) {
				wG = true;
				currWG = new WhirlpoolGenerator((bbReg->at("whirlpoolAppBB")), bbReg->at("particleSysBB")->center(), 0.7, 4);
				partSystem->addForceGenerator(currWG);
			}
			else {
				message::Message m((int)message::msgID::_m_GENERATOR_ERASABLE);
				m.genData.fg = currWG;
				message::MessageManager::sendMessage(m);
				wG = false;
			}
			break;
		}
		default:
			break;
	}
#endif // PROJECT
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	if (startCounting && (actor1 == martillo->getBallActor() || actor2 == martillo->getBallActor()) && (actor1 == field->getFieldActor() || actor2 == field->getFieldActor())) {
		_bbReg->addRegistry("hitBB", new BoundingBox(martillo->getBallPos() - Vector3(5, 5, 5), martillo->getBallPos() + Vector3(5, 5, 5)));
		_hammerHit = new ParticleSystem(*_bbReg->at("hitBB"));
		_hammerHit->addParticleGenerator(new HammerImpactGaussianGenerator("impact", _bbReg->at("hitBB")->center()));
		startCounting = false;
		maximaPuntuacion = std::max<float>(maximaPuntuacion, martillo->getDistance());
		textManager[tirada + 1]->setColor(colorutils::hexToVec4(0xffff00));
		finalRonda = true;
	}
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