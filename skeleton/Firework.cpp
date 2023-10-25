#include "Firework.h"

std::list<Particle*> Firework::explode() {
	std::list<Particle*> tmp;
	if (this->gen > 0) {
		FireworkGenerator* fG = new FireworkGenerator(this, this->getData().pose.p, Vector3(0.5,1,0.5), Vector3(0,50,0), Vector3(5, 10, 5), 5.0f, 5.0f);
		tmp = fG->generateParticles(rand() % 10 + 1);
		delete fG;
	}
	return tmp;
}