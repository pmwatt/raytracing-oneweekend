#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>


// raytracing sphere

// returns t (scalar from the camera A to a point along the direction vector b)
double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;

	// based on quadratic formula for finding t that hits the sphere's surface
	auto a = r.direction().length_squared(); // ray origin
	auto half_b = dot(oc, r.direction()); // ray direction
	auto c = oc.length_squared() - radius * radius; // sphere centre
	auto discriminant = half_b*half_b - a * c;

	// no root/solution for the quadratic eq if discriminant < 0
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		// root equation, sqrt may be expensive but we'll just do it once
		// consider +ve and -ve discriminant, only 1 sphere for now
		return (-half_b - sqrt(discriminant)) / a;
	}
}


color ray_color(const ray& r) {
	// t on P(t) = A + bt
	// sphere center's z can be switched into either {1, -1}, this solution
	// doesn't distinguish between objects in front/behind the camera (not a feature)
	auto t = hit_sphere(point3(0, 0, -1), 0.5, r);

	// if intersection t goes away from the camera onto the viewport
	if (t > 0.0) {
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1)); // sphere normal = intersection direction - sphere centre
		// don't have light yet, visualize normals with color map by assuming normal is a unit length vector
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1); // shade based on position
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);

	// lerp
	// blendedValue = ((1.0-a) * startValue) + (a  * endValue)
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	
}


int main() {

	// ray
	// for confusion about real value stuff, see their types: int vs double

	auto aspect_ratio = 16. / 9.;
	int image_width = 400;

	// calculate the image height, and ensure that it's at least 1 (int get truncated to 0 otherwise)
	int image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// camera

	auto focal_length = 1.0; // distance between the viewport and camera centre point
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
	auto camera_centre = point3(0, 0, 0); // eyepoint

	// horizontal/vertical vectors for navigating the pixel grid
	// origin at the top left
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0); // image coordinate's y is inverted of viewport

	// horizontal/vertical pixel delta vectors from pixel-pixel
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	// camera centre = viewport centre
	// minus focal length to get the camera to the viewport's plane
	// negative vector = opposite direction, we're moving the camera up, and to the left side of the viewport
	auto viewport_upper_left = camera_centre - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

	// location of the upper left pixel (image coordinates origin)
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// Render

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	// writes from left to right, top to bottom
	for (int j = 0; j < image_height; ++j) {
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v); // for indexing each pixel centre on the viewport
			auto ray_direction = pixel_center - camera_centre; // consider ray dir as a unit vector
			
			ray r(camera_centre, ray_direction); // P(t) = A +bt
			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);

		}
	}
	std::clog << "\rDone.			\n";
}
