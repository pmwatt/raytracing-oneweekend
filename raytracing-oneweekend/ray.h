#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
	ray() {}

	ray (const point3& origin, const vec3& direction) :
		orig(origin), dir(direction) {}

	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }

	// P(t) = A + tB at some t
	point3 at(double t) const {
		return orig + t * dir;
	}
 
private:
	// P(t) = A + tb
	point3 orig; // A
	vec3 dir; // b
};

#endif