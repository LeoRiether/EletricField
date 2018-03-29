#ifndef CMATH_H
#define CMATH_H
#include <cmath>
#endif

#ifndef VECTOR_H
#define VECTOR_H
#include "vector.h"
#endif

#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif

class Particle {
public:
	Vector pos;
	float q;
	
	Particle(Vector, const float&);
	Particle();
	
	Vector getFieldAt(const Vector&, float&, float&, float&);
};
	