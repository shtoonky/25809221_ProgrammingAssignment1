#ifndef SCENE_HPP
#define SCENE_HPP

#include "raytracer/hittable_list.hpp"
#include "raytracer/camera.hpp"
#include "result.hpp"
#include "error_type.hpp"

#include <string>
// #include <sstream>

class Scene {
    public:
        std::string scene_name;
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;

        HittableList world;
        Camera cam;

        void Initialise();

        void Show();

        bool HasObject(const std::string& name);

        Result<void, ErrorType> AddNewObject(const std::string& object_type, const std::string& object_name);
};

#endif