#pragma once
#include "Particle.h"

class Projectile : public Particle {
public:
	enum class ProjectileType { PROJECTILE_BULLET, PROJECTILE_CANNONBALL };
private:
	ProjectileType type;
	float mass;
public:

	Projectile(Vector3 Pos, Vector3 VelS, ProjectileType type);
	~Projectile() = default;
};

