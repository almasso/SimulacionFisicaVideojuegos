#include "ParticleGenerator.h"
#include "../checkMemoryLeaks.h"
#include "../Particle/Firework.h"
#include "../Utils/ColorUtils.h"
#include "../checkMemoryLeaks.h"

ParticleGenerator::~ParticleGenerator() {
	for (auto it = models.begin(); it != models.end();) {
		if ((*it)) {
			delete (*it);
			it = models.erase(it);
		}
		else ++it;
	}
}

GaussianParticleGenerator::GaussianParticleGenerator(Particle::Particle_Type type, Particle::Particle_Shape shape, std::string name, const Point& generationPosition, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed, float averageLifeTime, float sigmaLifeTime) : ParticleGenerator(name, generationPosition, averageSpeed, averageLifeTime),
	gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())), sigmaPos(sigmaPos), sigmaVel(sigmaSpeed)
{
	pX = new std::normal_distribution<float>(avrg_pos.x, sigmaPos.x);
	pY = new std::normal_distribution<float>(avrg_pos.y, sigmaPos.y);
	pZ = new std::normal_distribution<float>(avrg_pos.z, sigmaPos.z);
	vX = new std::normal_distribution<float>(avrg_vel.x, sigmaSpeed.x);
	vY = new std::normal_distribution<float>(avrg_vel.y, sigmaSpeed.y);
	vZ = new std::normal_distribution<float>(avrg_vel.z, sigmaSpeed.z);
	t = new std::normal_distribution<float>(avrg_lifeTime, sigmaLifeTime);

	for (int i = 0; i < 10; ++i) {
		Particle::Particle_Shape tmp;
		shape == Particle::Particle_Shape::MIX ? tmp = (Particle::Particle_Shape)(rand() % (int)(Particle::Particle_Shape::MIX)) : tmp = shape;
		setParticle(new Particle(type, tmp, 0.998, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1)));
	}
}

GaussianParticleGenerator::~GaussianParticleGenerator() {
	delete pX;
	delete pY;
	delete pZ;
	delete vX;
	delete vY;
	delete vZ;
	delete t;
}

std::list<Particle*> GaussianParticleGenerator::generateParticles(int numParticles) {
	std::list<Particle*> tmp;
	for (int i = 0; i < numParticles; ++i) {
		Particle::particle_data data = models[rand() % models.size()]->getData();
		Particle* partTmp = new Particle(data.type, data.shape, 1.0, Vector3((*pX)(gen), (*pY)(gen), (*pZ)(gen)), Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen)), data.damping, data.colour);
		partTmp->setLifeTime((*t)(gen));
		tmp.push_back(partTmp);
	}
	return tmp;
}

HammerImpactGaussianGenerator::HammerImpactGaussianGenerator(std::string name, const Point& generationPosition) : GaussianParticleGenerator(Particle::Particle_Type::NORMAL, Particle::Particle_Shape::MIX, name, generationPosition, Vector3(0.01f, 0.001f, 0.01f), Vector3(0, 50, 0.0f), Vector3(15, 20, 15), 5.0f, 5.0f) {
	for (auto it = models.begin(); it != models.end();) {
		if ((*it)) {
			delete (*it);
			it = models.erase(it);
		}
		else ++it;
	}
	setParticle(new Particle(Particle::Particle_Type::NORMAL, Particle::Particle_Shape::CUBE, 0.998, colorutils::hexToVec4(0x009A17)));
	setParticle(new Particle(Particle::Particle_Type::NORMAL, Particle::Particle_Shape::SPHERE, 0.998, colorutils::hexToVec4(0x009A17)));
}

GaussianSolidParticleGenerator::GaussianSolidParticleGenerator(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Particle::Particle_Type type, Particle::Particle_Shape shape, std::string name, const Point& generationPosition, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed, float averageLifeTime, float sigmaLifeTime) : GaussianParticleGenerator(type, shape, name, generationPosition, sigmaPos, averageSpeed, sigmaSpeed, averageLifeTime, sigmaLifeTime), gPhysics(gPhysics), gScene(gScene) {
	for (auto it = models.begin(); it != models.end();) {
		if ((*it)) {
			delete (*it);
			it = models.erase(it);
		}
		else ++it;
	}

	for (int i = 0; i < 10; ++i) {
		Particle::Particle_Shape tmp;
		shape == Particle::Particle_Shape::MIX ? tmp = (Particle::Particle_Shape)(rand() % (int)(Particle::Particle_Shape::MIX)) : tmp = shape;
		setParticle(new SolidParticle(gPhysics, gScene, type, tmp, 0.998, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1)));
	}
}

std::list<Particle*> GaussianSolidParticleGenerator::generateParticles(int numParticlesToGenerate) {
	std::list<Particle*> tmp;
	if (current < max) {
		for (int i = 0; i < numParticlesToGenerate; ++i) {
			Particle::particle_data data = models[rand() % models.size()]->getData();
			SolidParticle* partTmp = new SolidParticle(gPhysics, gScene, data.type, data.shape, 1.0, Vector3((*pX)(gen), (*pY)(gen), (*pZ)(gen)), Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen)), data.damping, data.colour);
			partTmp->setParticleGenerator(this);
			partTmp->setLifeTime((*t)(gen));
			tmp.push_back(partTmp);
		}
		current += numParticlesToGenerate;
	}
	return tmp;
}

UniformParticleGenerator::UniformParticleGenerator(Particle::Particle_Type type, Particle::Particle_Shape shape, std::string name, const Point& pA, const Point& pB, const Vector3& sA, const Vector3& sB, float tA, float tB) : ParticleGenerator(name, pA - pB, sA - sB, tA - tB),
gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())) {
	pX = new std::uniform_real_distribution<float>(min(pA.x, pB.x), max(pA.x, pB.x));
	pY = new std::uniform_real_distribution<float>(min(pA.y, pB.y), max(pA.y, pB.y));
	pZ = new std::uniform_real_distribution<float>(min(pA.z, pB.z), max(pA.z, pB.z));
	vX = new std::uniform_real_distribution<float>(min(sA.x, sB.x), max(sA.x, sB.x));
	vY = new std::uniform_real_distribution<float>(min(sA.y, sB.y), max(sA.y, sB.y));
	vZ = new std::uniform_real_distribution<float>(min(sA.z, sB.z), max(sA.z, sB.z));
	t = new std::uniform_real_distribution<float>(min(tA, tB), max(tA, tB));

	for (int i = 0; i < 10; ++i) {
		Particle::Particle_Shape tmp;
		shape == Particle::Particle_Shape::MIX ? tmp = (Particle::Particle_Shape)(rand() % (int)(Particle::Particle_Shape::MIX)) : tmp = shape;
		setParticle(new Particle(type, tmp, 0.998, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1)));
	}
}

UniformParticleGenerator::~UniformParticleGenerator() {
	delete pX;
	delete pY;
	delete pZ;
	delete vX;
	delete vY;
	delete vZ;
	delete t;
}

std::list<Particle*> UniformParticleGenerator::generateParticles(int numParticles) {
	std::list<Particle*> tmp;
	for (int i = 0; i < numParticles; ++i) {
		Particle::particle_data data = models[rand() % models.size()]->getData();
		Particle* partTmp = new Particle(data.type, data.shape, 1, Vector3((*pX)(gen), (*pY)(gen), (*pZ)(gen)), Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen)), data.damping, data.colour);
		partTmp->setLifeTime((*t)(gen));
		tmp.push_back(partTmp);
	}
	return tmp;
}

UniformSolidParticleGenerator::UniformSolidParticleGenerator(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Particle::Particle_Type type, Particle::Particle_Shape shape, std::string name, const Point& pA, const Vector3& pB, const Vector3& sA, const Vector3& sB, float tA, float tB) : UniformParticleGenerator(type, shape, name, pA, pB, sA, sB, tA, tB), gPhysics(gPhysics), gScene(gScene) {
	for (auto it = models.begin(); it != models.end();) {
		if ((*it)) {
			delete (*it);
			it = models.erase(it);
		}
		else ++it;
	}

	for (int i = 0; i < 10; ++i) {
		Particle::Particle_Shape tmp;
		shape == Particle::Particle_Shape::MIX ? tmp = (Particle::Particle_Shape)(rand() % (int)(Particle::Particle_Shape::MIX)) : tmp = shape;
		setParticle(new SolidParticle(gPhysics, gScene, type, tmp, 0.998, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1)));
	}
}

std::list<Particle*> UniformSolidParticleGenerator::generateParticles(int numParticles) {
	std::list<Particle*> tmp;
	if (current < max) {
		for (int i = 0; i < numParticles; ++i) {
			Particle::particle_data data = models[rand() % models.size()]->getData();
			SolidParticle* partTmp = new SolidParticle(gPhysics, gScene, data.type, data.shape, 1, Vector3((*pX)(gen), (*pY)(gen), (*pZ)(gen)), Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen)), data.damping, data.colour);
			partTmp->setParticleGenerator(this);
			partTmp->setLifeTime((*t)(gen));
			tmp.push_back(partTmp);
		}
		current += numParticles;
	}
	return tmp;
}



FireworkGenerator::FireworkGenerator(Firework* parent) : ParticleGenerator("fireworkGen", Vector3(0,0,0), Vector3(0,0,0), 0), parent(parent) {
	for (int i = 0; i < 10; ++i) {
		models.push_back(new Firework(parent->getGen(), rand() % 1000 / 1000.0f, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1)));
	}
}

std::list<Particle*> FireworkGenerator::generateParticles(int numParticles) {
	return generateParticles(parent, numParticles);
}

std::list<Particle*> FireworkGenerator::generateParticles(Firework* parent, int numParticles) {
	std::list<Particle*> tmp;
	for (int i = 0; i < numParticles; ++i) {
		Particle::particle_data data = models[rand() % models.size()]->getData();
		Firework* fireworkTmp = new Firework(static_cast<Firework*>(models[rand() % models.size()])->getGen() - 1, parent->getPosition().p, Vector3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50).getNormalized() * 25, data.damping, data.colour, true);
		tmp.push_back(fireworkTmp);
	}
	return tmp;
}