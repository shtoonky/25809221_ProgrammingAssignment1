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
#include <typeinfo>

class Scene {
    public:
        // Aspects of the scene the user can modify.
        std::string scene_name;
        double aspect_ratio = 1.0;;
        int image_width = 100;
        int samples_per_pixel = 10;
        // List of objects in the scene.
        HittableList world;

        Camera cam;

        void Initialise() {

            // world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
            // world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

            cam.aspect_ratio = aspect_ratio;
            cam.image_width = image_width;
            // cam.samples_per_pixel = samples_per_pixel;
            cam.Initialise();
        }

        void ShowScene() {
        
            std::cout << '\n' << "Scene name: " << scene_name << '\n';
            std::cout << "Aspect ratio: " << aspect_ratio << '\n';
            std::cout << "Image width: " << image_width << '\n';
            std::cout << "Anti-aliasing strength: " << samples_per_pixel << "\n\n";

            for (const auto& object : world.objects) {
                std::cout<< "printing";
                std::cout << "Name:[" <<object -> name << ']' << '\n';
            }

            std::cout << '\n';
        }

        // Then render using filemanager
};

#endif