#include "Projectile.h"
#include "../checkMemoryLeaks.h"

Projectile::Projectile(Vector3 Pos, Vector3 Dir, float VelS, float size, ProjectileType type, Vector4 Col) : Particle(Particle::Particle_Type::PROJECTILE, 0.0, Pos, Dir * VelS, size, 0.998f, Col), type(type) {
	switch (type) {
		case ProjectileType::PROJECTILE_BULLET: {
			//Para una bala de aproximadamente 0,02kg tenemos que iría a una velocidad de 330m/s, por lo que podemos
			//sacar su aceleración y su nueva masa simulada a partir de de la velocidad simulada
			//MasaReal * velReal^2 = MasaSim * velSim^2
			mass = 0.02 * powf((330) / VelS, 2);
			float inv_mass = 1 / mass;
			this->data.inv_mass = inv_mass;
			//Para calcular la gravedad, tenemos que VelReal^2/gR = VelSim^2/gS, por tanto gS = gR(Vsim/Vreal)^2
			//this->setAcceleration(Vector3(0, -9.8f * powf((VelS/330),2), 0));
			break;
		};

		case ProjectileType::PROJECTILE_CANNONBALL: {
			//Para una bola de cañón de aproximadamente 20kg tenemos que iría a una velocidad de 250m/s. Calculamos lo mismo:
			mass = 20 * powf((250) / VelS, 2);
			float inv_mass = 1 / mass;
			this->data.inv_mass = inv_mass;
			//this->setAcceleration(Vector3(0, -9.8f * powf((VelS/250), 2), 0));
			break;
		};				
	}
}
