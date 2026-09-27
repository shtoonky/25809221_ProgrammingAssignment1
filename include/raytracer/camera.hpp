#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "rtweekend_utils.hpp"
#include "colour.hpp"
#include "hittable.hpp"

class Camera {
    public:
        double aspect_ratio;
        int image_width;
        int samples_per_pixel;
        int max_depth; // Maximum number of ray bounces into scene

        Colour skybox_colour_i = Colour(0, 0, 0);
        Colour skybox_colour_j = Colour(1, 1, 1);
        
        Camera() {}
        Camera(double ar, int w) : aspect_ratio(ar), image_width(w) {}

        void Initialise() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = Point3(0, 0, 0);

            // Determine viewport dimensions.
            double focal_lenth = 1.0;
            double viewport_height = 2.0;
            double viewport_width = viewport_height * (double(image_width) / image_height);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            auto viewport_u = Vec3(viewport_width, 0, 0);
            auto viewport_v = Vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left = center - Vec3(0, 0, focal_lenth) - viewport_u / 2 - viewport_v / 2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        void Render(std::ostream& output, const Hittable& world) const {
            output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {

                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

                for (int i = 0; i < image_width; i++) {
                    Colour pixel_colour = Colour (0, 0, 0);
                    if (samples_per_pixel == 0) {   // If anti-aliasing is 'off'
                        auto pixel_center = pixel00_loc + (i * pixel_delta_u + (j * pixel_delta_v));
                        auto ray_direction = pixel_center - center;
                        Ray r(center, ray_direction);

                        pixel_colour = RayColour(r, max_depth, world);
                        WriteColour(output, pixel_colour);
                    }
                    else { // Anti-aliasing on
                        pixel_colour = Colour(0, 0, 0);

                        for (int sample = 0; sample < samples_per_pixel; sample++) {
                            Ray r = GetRay(i, j);
                            pixel_colour += RayColour(r, max_depth, world);
                        }
                        WriteColour(output, pixel_colour * pixel_samples_scale);
                    }
                }
            }
            std::clog << "\r                             ";          
        }

        Ray GetRay(int i, int j) const {
            // Construct a camera ray originating from the origin and directed at randomly sampled points around the pixel location i, j.

            auto offset = SampleSquare();
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return Ray(ray_origin, ray_direction);
        }

        Vec3 SampleSquare() const {
            // Returns a Vec3 to a random point in the [-.5, -.5]-[+.5, +.5] unit square.
            return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
        }

        Colour RayColour(const Ray& r, int depth, const Hittable& world) const {
            HitRecord rec;

            if (depth <= 0)
                return Colour(0, 0, 0);

            if (world.Hit(r, Interval(0.001, infinity), rec)) {
                Vec3 direction = RandomOnHemisphere(rec.normal);
                return 0.5 * RayColour(Ray(rec.p, direction), depth - 1, world);
            }

            Vec3 unit_direction = UnitVector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * skybox_colour_i + a * skybox_colour_j;
        }
    private:
        int image_height;
        double pixel_samples_scale;
        Point3 center;
        Point3 pixel00_loc;
        Vec3 pixel_delta_u;
        Vec3 pixel_delta_v;
};

#endif