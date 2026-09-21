#ifndef SCENE_HPP
#define SCENE_HPP

#include "camera.hpp"
#include "hittable_list.hpp"

#include <string>

class Scene {
    public:
        std::string scene_name;
        int image_width;

        HittableList world;

        // for each object in a scene txt (returned as a vector or something), add it to world OR world equals that.

        Camera cam;

        // set things like aspect ratio
        // image_width
        //samples per pixel

        // then render

        // Ideally, we want to output a ppm to a folder, then maybe auto open it? i don't know if that's possible :|
        void RenderScene() {
            cam.Render(world);
        }

        // Vector of hittable objects
        // Anti alias settings
        // image size
};

#endif