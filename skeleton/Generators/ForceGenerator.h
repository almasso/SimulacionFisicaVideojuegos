#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"
#include "../Utils/BoundingBox.h"
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
	Vector3 _windSpeed;
	BoundingBox zone;
public:
	ParticleDragGenerator(BoundingBox& zone, Vector3 windSpeed, const float k1, const float k2) : _windSpeed(windSpeed), zone(zone) { setDrag(k1, k2); }
	void updateForce(Particle* particle, double duration) override;
	inline void setDrag(float k1, float k2) { _k1 = k1; _k2 = k2; }
	inline float getK1() const { return _k1; }
	inline float getK2() const { return _k2; }
};

class WhirlpoolGenerator : public ForceGenerator {
private:
	Vector3 _whirlpoolSpeed;
	Vector3 _whirlpoolCenter;
	float _k;
	float _k1;
	BoundingBox zone;
public:
	WhirlpoolGenerator(BoundingBox& zone, Vector3 center, float K, float k1) : _whirlpoolCenter(center), zone(zone), _k(K), _k1(k1) {}
	void updateForce(Particle* particle, double duration) override;
};

class ExplosionGenerator : public ForceGenerator {
private:
	Vector3 explosionCenter;
	float explosionRadius = 20;
	float K;
	float tau;
	static Vector3 explosionSpeed;
public:
	ExplosionGenerator(Vector3 explosionCenter, float K, float tau) : explosionCenter(explosionCenter), explosionRadius(explosionRadius), K(K), tau(tau) {}
	void updateForce(Particle* particle, double duration) override;
};

class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particle* other) : _k(k), _resting_length(resting_length), _other(other) {}
	void updateForce(Particle*, double duration) override;
	inline void setK(double k) { _k = k; }
protected:
	double _k;
	double _resting_length;
	Particle* _other;
};

class AnchoredSpringFG : public SpringForceGenerator {
public:
	AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos);
	~AnchoredSpringFG() {
		delete _other;
	}
};

