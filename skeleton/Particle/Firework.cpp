#include "Firework.h"

std::list<Particle*> Firework::explode() {
	std::list<Particle*> tmp;
	if (this->gen > 0) {
		FireworkGenerator* fG = new FireworkGenerator(this);
		tmp = fG->generateParticles(rand() % 10 + 1);
		delete fG;
	}
	return tmp;
}