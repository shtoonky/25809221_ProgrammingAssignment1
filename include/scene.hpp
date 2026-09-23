#ifndef SCENE_HPP
#define SCENE_HPP

#include "raytracer/colour.hpp"
#include "raytracer/vec3.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/camera.hpp"
// #include "file_manager.hpp"
// #include "error_type.hpp"

#include <string>

class Scene {
    public:
        std::string scene_name;
        double aspect_ratio = 1.0;;
        int image_width = 100;
        
        int samples_per_pixel = 10;

        HittableList world;
        Camera cam;

        void Initialise() {

            world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
            world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

            cam.aspect_ratio = aspect_ratio;
            cam.image_width = image_width;
            cam.Initialise();
        }

        // Then render using filemanager
};

#endif