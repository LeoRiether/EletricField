#ifndef VECTOR_H
#define VECTOR_H
#include "vector.h"
#endif

Vector::Vector() {
	this->x = 0;
	this->y = 0;
}

Vector::Vector(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector Vector::operator+(Vector& a) {
	return Vector(this->x + a.x, this->y + a.y);
}

float Vector::abs() {
	// Couldn't use ndist for some reason
	return sqrt(this->x*this->x + this->y*this->y);
}

