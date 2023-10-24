#include "ParticleGenerator.h"
GaussianParticleGenerator::GaussianParticleGenerator(std::string name, const Point& generationPosition, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed, float averageLifeTime, float sigmaLifeTime) : ParticleGenerator(name, generationPosition, averageSpeed, averageLifeTime), 
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
		setParticle(new Particle(0.998, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1), true));
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

std::list<Particle*> GaussianParticleGenerator::generateParticles() {
	std::list<Particle*> tmp;
	Particle* partTmp = new Particle(*models[rand() % models.size()]);
	partTmp->setPosition(Vector3((*pX)(gen), (*pY)(gen), (*pZ)(gen)));
	partTmp->setVelocity(Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen)));
	partTmp->setLifeTime((*t)(gen));
	tmp.push_back(partTmp);
	return tmp;
}

UniformParticleGenerator::UniformParticleGenerator(std::string name, const Point& generationPosition, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed, float averageLifeTime, float sigmaLifeTime) : ParticleGenerator(name, generationPosition, averageSpeed, averageLifeTime),
gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())), sigmaPos(sigmaPos), sigmaVel(sigmaSpeed)
{
	pX = new std::uniform_real_distribution<float>(avrg_pos.x, sigmaPos.x);
	pY = new std::uniform_real_distribution<float>(avrg_pos.y, sigmaPos.y);
	pZ = new std::uniform_real_distribution<float>(avrg_pos.z, sigmaPos.z);
	vX = new std::uniform_real_distribution<float>(avrg_vel.x, sigmaSpeed.x);
	vY = new std::uniform_real_distribution<float>(avrg_vel.y, sigmaSpeed.y);
	vZ = new std::uniform_real_distribution<float>(avrg_vel.z, sigmaSpeed.z);
	t = new std::uniform_real_distribution<float>(avrg_lifeTime, sigmaLifeTime);

	for (int i = 0; i < 10; ++i) {
		setParticle(new Particle(0.998, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1), true));
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