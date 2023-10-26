#include "ParticleGenerator.h"
#include "checkMemoryLeaks.h"
#include "Firework.h"

ParticleGenerator::~ParticleGenerator() {
	for (auto it = models.begin(); it != models.end();) {
		if ((*it)) {
			delete (*it);
			it = models.erase(it);
		}
		else ++it;
	}
}

GaussianParticleGenerator::GaussianParticleGenerator(Particle::Particle_Type type, std::string name, const Point& generationPosition, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed, float averageLifeTime, float sigmaLifeTime) : ParticleGenerator(name, generationPosition, averageSpeed, averageLifeTime),
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
		setParticle(new Particle(type, 0.998, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1)));
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
		Particle* partTmp = new Particle(data.type, Vector3((*pX)(gen), (*pY)(gen), (*pZ)(gen)), Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen)), data.damping, data.colour, true);
		partTmp->setLifeTime((*t)(gen));
		tmp.push_back(partTmp);
	}
	return tmp;
}

UniformParticleGenerator::UniformParticleGenerator(Particle::Particle_Type type, std::string name, const Point& pA, const Point& pB, const Vector3& sA, const Vector3& sB, float tA, float tB) : ParticleGenerator(name, pA - pB, sA - sB, tA - tB),
gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())) {
	pX = new std::uniform_real_distribution<float>(min(pA.x, pB.x), max(pA.x, pB.x));
	pY = new std::uniform_real_distribution<float>(min(pA.y, pB.y), max(pA.y, pB.y));
	pZ = new std::uniform_real_distribution<float>(min(pA.z, pB.z), max(pA.z, pB.z));
	vX = new std::uniform_real_distribution<float>(min(sA.x, sB.x), max(sA.x, sB.x));
	vY = new std::uniform_real_distribution<float>(min(sA.y, sB.y), max(sA.y, sB.y));
	vZ = new std::uniform_real_distribution<float>(min(sA.z, sB.z), max(sA.z, sB.z));
	t = new std::uniform_real_distribution<float>(min(tA, tB), max(tA, tB));

	for (int i = 0; i < 10; ++i) {
		setParticle(new Particle(type, 0.998, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1)));
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
		Particle* partTmp = new Particle(data.type, Vector3((*pX)(gen), (*pY)(gen), (*pZ)(gen)), Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen)), data.damping, data.colour, true);
		partTmp->setLifeTime((*t)(gen));
		tmp.push_back(partTmp);
	}
	return tmp;
}

FireworkGenerator::FireworkGenerator(Firework* parent) : ParticleGenerator("fireworkGen", Vector3(0,0,0), Vector3(0,0,0), 0), parent(parent) {
	for (int i = 0; i < 10; ++i) {
		models.push_back(new Firework(rand() % 3 + 1, rand() % 1000 / 1000.0f, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1)));
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