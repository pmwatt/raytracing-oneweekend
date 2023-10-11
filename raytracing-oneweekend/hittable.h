#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record
{
public:
	point3 p; // intersection
	vec3 normal;
	double t;
	bool front_face;


};

class hittable
{
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif