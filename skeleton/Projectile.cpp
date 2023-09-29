#include "Projectile.h"

Projectile::Projectile(Vector3 Pos, Vector3 VelS, ProjectileType type) : Particle(Pos, VelS), type(type) {
	switch (type) {
		case ProjectileType::PROJECTILE_BULLET: {
			//Para una bala de aproximadamente 0,02kg tenemos que iría a una velocidad de 330m/s, por lo que podemos
			//sacar su aceleración y su nueva masa simulada a partir de de la velocidad simulada
			//MasaReal * velReal^2 = MasaSim * velSim^2
			mass = 0.02 * powf((330) / VelS.magnitude(), 2);
			//Para calcular la gravedad, tenemos que VelReal^2/gR = VelSim^2/gS, por tanto gS = gR(Vsim/Vreal)^2
			this->setAcceleration(Vector3(0, -9.8f * powf((VelS.magnitude()/330),2), 0));
		};
		case ProjectileType::PROJECTILE_CANNONBALL: {
			this->setAcceleration(Vector3());
		};				
	}
}
