#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>


int main() {

	// ray
	// for confusion about real value stuff, see their types: int vs double

	auto aspect_ratio = 16. / 9.; // ideal ratio
	int image_width = 400;

	// calculate the image height, and ensure that it's at least 1
	int image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height; // in case height < 1, int will get truncated to 0

	// viewport width < 1 is ok, viewport is a real number (double)
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

	// Image

	int image_width = 256;
	int image_height = 256;

	// Render

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = 0; j < image_height; ++j) {
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
			write_color(std::cout, pixel_color);
		}
	}
	std::clog << "\rDone.			\n";
}
