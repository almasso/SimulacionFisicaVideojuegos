#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"
using Force = Vector3;
class Particle;

class ForceGenerator {
public:
	virtual void updateForce(Particle* particle, double duration) = 0;

	inline bool updateTime(double t) {
		_t += t;
		return _t < _duration || _duration < 0.0;
	}
	virtual ~ForceGenerator() = default;
protected:
	std::string _name;
	double _t = 0.0;
	double _duration = -1e10;
};

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& g) : _gravity(g) {}
	void updateForce(Particle* particle, double duration) override;
	inline void setGravity(Vector3 g) { _gravity = g; }
protected:
	Vector3 _gravity;
};

class ParticleDragGenerator : public ForceGenerator {
private:
	float _k1;
	float _k2;
public:
	ParticleDragGenerator(const float k1, const float k2) { setDrag(k1, k2); }
	void updateForce(Particle* particle, double duration) override;
	inline void setDrag(float k1, float k2) { _k1 = k1; _k2 = k2; }
	inline float getK1() const { return _k1; }
	inline float getK2() const { return _k2; }
};

