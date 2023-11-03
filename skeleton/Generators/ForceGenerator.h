#pragma once
#include "../RenderUtils.hpp"
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

