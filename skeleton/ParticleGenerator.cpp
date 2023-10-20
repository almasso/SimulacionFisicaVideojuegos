#include "ParticleGenerator.h"
GaussianParticleGenerator::GaussianParticleGenerator(std::string name, const BoundingBox& bb, const Point& generationPosition, const Vector3& averagePos, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed) : ParticleGenerator(name, bb, averagePos, averageSpeed), 
	gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())), sigmaPos(sigmaPos), sigmaVel(sigmaSpeed)
{
	pX = new std::normal_distribution<float>(avrg_pos.x, sigmaPos.x);
	pY = new std::normal_distribution<float>(avrg_pos.y, sigmaPos.y);
	pZ = new std::normal_distribution<float>(avrg_pos.z, sigmaPos.z);
	vX = new std::normal_distribution<float>(avrg_vel.x, sigmaSpeed.x);
	vY = new std::normal_distribution<float>(avrg_vel.y, sigmaSpeed.y);
	vZ = new std::normal_distribution<float>(avrg_vel.z, sigmaSpeed.z);

	setParticle(new Particle());
}

GaussianParticleGenerator::~GaussianParticleGenerator() {
	delete pX;
	delete pY;
	delete pZ;
	delete vX;
	delete vY;
	delete vZ;
}

std::list<Particle*> GaussianParticleGenerator::generateParticles() {

}