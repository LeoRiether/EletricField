#ifndef CMATH_H
#define CMATH_H
#include <cmath>
#endif

class Vector {
public:
	float x;
	float y;
	
	Vector();
	Vector(float, float);
	
	Vector operator+(Vector&);
	
	float abs();
};

