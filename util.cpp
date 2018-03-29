#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif

// Distance between two vectors
float dist(const Vector& a, const Vector& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrt(dx*dx + dy*dy); 
}

// Distance between two vectors, but with different arguments
float ndist(const int& ax, const int& ay, const int& bx, const int& by) {
	int dx = ax - bx;
	int dy = ay - by;
	return sqrt(dx*dx + dy*dy); 
}