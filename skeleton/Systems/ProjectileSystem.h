#pragma once
#include <list>
#include <string>
#include <vector>
#include "../Particle/Projectile.h"
#include "../Utils/BoundingBox.h"
#include "../Registries/ParticleForceRegistry.h"
#include "../Generators/ForceGenerator.h"
#include "../checkMemoryLeaks.h"

const float tiempo_borrado_proj = 40.0f;

class ProjectileSystem {
protected:
	std::list<Projectile*> _particles;
	ParticleForceRegistry _particleRegistry;
	std::vector<ForceGenerator*> _forces;

	BoundingBox _bb;
public:
	ProjectileSystem(const BoundingBox& bb) : _bb(bb) {}
	~ProjectileSystem();
	void update(double t);
	inline void addForceGenerator(ForceGenerator* fG) { _forces.push_back(fG); }
	void generateProjectile(Projectile* p);
};

