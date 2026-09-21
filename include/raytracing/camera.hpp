#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "hittable.hpp"

class Camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100; // Rendered image width pixel count
        int samples_per_pixel = 10; // Count of random samples for each pixel (anti-aliasing)

        void Render(const Hittable& world) {
            Initialise();
        }

    private:
        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        Vec3 pixel_delta_u;
        Vec3 pixel_delta_v;

        void Initialise() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height; // image_height cannot be < 1

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = point3(0, 0, 0);

            // Determine viewport dimensions.
            double focal_length = 1.0;
            double viewport_height = 2.0;
            double viewport_width = viewport_height * (double(image_width) / image_height);

            // Calcuate the vectors across the horizontal and down the vertical edges.
            auto viewport_u = Vec3(viewport_width, 0, 0);
            auto viewport_v = Vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left = center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }
};

#endif