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

	void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		// set hit record normal vector
		// outward_normal assumed to have unit length so that dot = cos(angle)

		// 2 options: normal always point against the ray, or normal always point out from the centre
		// we chose the former - normal always point against the ray
		front_face = dot(r.direction(), outward_normal) < .0; // opposite direction i.e. normal facing against the ray?
		normal = front_face ? outward_normal : -outward_normal; // make non-front face point against the ray direction
	}
};

class hittable
{
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif