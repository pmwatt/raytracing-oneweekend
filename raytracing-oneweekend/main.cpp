#include "rtweekend.h" // contains ray and vec3

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>


// raytracing sphere



color ray_color(const ray& r, const hittable& world) {
	hit_record rec;

	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rec.normal + color(1,1,1)); // shade based on xyz
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

	// world

	hittable_list world;
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(1.1,0.4,-0.7), 0.2));

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
			color pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);

		}
	}
	std::clog << "\rDone.			\n";
}
