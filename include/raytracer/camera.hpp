#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "rtweekend_utils.hpp"
#include "colour.hpp"
#include "hittable.hpp"

class Camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;
        
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

        Colour RayColour(const Ray& r, const Hittable& world) const {
            HitRecord rec;

            if (world.Hit(r, Interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + Colour(1, 1, 1));
            }

            Vec3 unit_direction = UnitVector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
        }

        // Getters :|

        int GetImageHeight() const {
            return image_height;
        }

        double GetPixelSamplesScale() const {
            return pixel_samples_scale;
        }

        Point3 GetCenter() const {
            return center;
        }

        Point3 GetPixel00() const {
            return pixel00_loc;
        }

        Vec3 GetPixelDeltaU() const {
            return pixel_delta_u;
        }

        Vec3 GetPixelDeltaV() const {
            return pixel_delta_v;
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