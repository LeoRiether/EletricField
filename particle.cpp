#ifndef PARTICLE_H
#define PARTICLE_H
#include "particle.h"
#endif


Particle::Particle(Vector p, const float& q) {
	this->pos = p;
	this->q = q;
}

Particle::Particle() {
	this->pos = Vector(0,0);
	this->q = 0;
}

Vector Particle::getFieldAt(const Vector& p, float& d, float& theta, float& val) {
	d = dist(this->pos, p);
	theta = atan2(p.y-this->pos.y, p.x-this->pos.x);
	val = this->q / (d*d);  
	return Vector(val * cos(theta), val * sin(theta));	
}
