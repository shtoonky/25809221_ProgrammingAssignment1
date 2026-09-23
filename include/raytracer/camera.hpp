#ifndef CAMERA_HPP
#define CAMERA_HPP

// #include "colour.hpp"
#include "rtweekend_utils.hpp"
// #include "ray.hpp"
#include "hittable.hpp"

class Camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;

        int image_height;
        Point3 center;
        Point3 pixel00_loc;
        Vec3 pixel_delta_u;
        Vec3 pixel_delta_v;
        
        Camera() {}
        Camera(double ar, int w) : aspect_ratio(ar), image_width(w) {}

        // void Render(std::ostream& out, const Hittable& world) {
        //     Initialise();

        //     out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        //     for (int j = 0; j < image_height; j++) {

        //         for (int i = 0; i < image_width; i++) {
        //             auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        //             auto ray_direction = pixel_center - center;
        //             Ray r(center, ray_direction);

        //             Colour pixel_color = RayColour(r, world);
        //             WriteColour(out, pixel_color);
        //         }
        //     }
        // }
        void Initialise() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

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

        Colour RayColour(const Ray& r, const Hittable& world) const {
            HitRecord rec;

            if (world.Hit(r, 0, infinity, rec)) {
                return 0.5 * (rec.normal + Colour(1, 1, 1));
            }

            Vec3 unit_direction = UnitVector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
        }

    // private:


        // void Initialise() {
        //     image_height = int(image_width / aspect_ratio);
        //     image_height = (image_height < 1) ? 1 : image_height;

        //     center = Point3(0, 0, 0);

        //     // Determine viewport dimensions.
        //     double focal_lenth = 1.0;
        //     double viewport_height = 2.0;
        //     double viewport_width = viewport_height * (double(image_width) / image_height);

        //     // Calculate the vectors across the horizontal and down the vertical viewport edges.
        //     auto viewport_u = Vec3(viewport_width, 0, 0);
        //     auto viewport_v = Vec3(0, -viewport_height, 0);

        //     // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        //     pixel_delta_u = viewport_u / image_width;
        //     pixel_delta_v = viewport_v / image_height;

        //     // Calculate the location of the upper left pixel.
        //     auto viewport_upper_left = center - Vec3(0, 0, focal_lenth) - viewport_u / 2 - viewport_v / 2;
        //     pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        // }

        // Colour RayColour(const Ray& r, const Hittable& world) const {
        //     HitRecord rec;

        //     if (world.Hit(r, 0, infinity, rec)) {
        //         return 0.5 * (rec.normal + Colour(1, 1, 1));
        //     }

        //     Vec3 unit_direction = UnitVector(r.direction());
        //     auto a = 0.5 * (unit_direction.y() + 1.0);
        //     return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
        // }
};

#endif