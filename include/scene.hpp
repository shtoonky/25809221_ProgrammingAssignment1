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
        int max_depth = 10;

        Colour skybox_colour_i = Colour(0, 0, 0);
        Colour skybox_colour_j = Colour(1, 1, 1);

        HittableList world;
        Camera cam;

        void Initialise();

        void Show(bool editing=false);

        bool HasObject(const std::string& name);

        Result<shared_ptr<Hittable>, ErrorType> GetObject(const std::string& name);

        Result<void, ErrorType> AddNewObject(const std::string& object_type, const std::string& object_name);

        Result<void, ErrorType> RemoveObject(const std::string& name);

        Result<void, ErrorType> ModifyObject(shared_ptr<Hittable>& object,const std::string& variable,const std::string& value);

        std::vector<std::string> GetObjectNames();
};

#endif