#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	sphere(point3 _center, double _radius) :
		center(_center), radius(_radius) {}

	// modifies hit record rec
	// true if ray r hits this sphere while its t is between [tmin, tmax]
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{
		vec3 oc = r.origin() - center;

		// based on quadratic formula for finding t that hits the sphere's surface
		auto a = r.direction().length_squared(); // ray origin
		auto half_b = dot(oc, r.direction()); // ray direction
		auto c = oc.length_squared() - radius * radius; // sphere centre

		auto discriminant = half_b*half_b - a * c;
		if (discriminant < 0) return false; // no quadratic root
		auto sqrtd = sqrt(discriminant);

		// find the nearest root within specified range, +ve discriminant one preferred
		auto root = (-half_b - sqrtd) / a;
		if (!ray_t.surrounds(root))
			root = (-half_b + sqrtd) / a; // bigger +ve = always the root closest to the camera
			if (!ray_t.surrounds(root))
				return false;

		// update hit record
		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius; // (P-C)/radius unit vector
		rec.set_face_normal(r, outward_normal); // also distinguish front/back face for the normal
		return true;
	}

private:
	point3 center;
	double radius;
};

#endif