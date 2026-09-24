#ifndef SCENE_HPP
#define SCENE_HPP

#include "raytracer/colour.hpp"
#include "raytracer/vec3.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/camera.hpp"
#include "error_type.hpp"

#include <string>
#include <typeinfo>
#include <sstream>
// #include <sstream>

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
        Scene() : scene_name("") {}
        Scene(const std::string& name) : scene_name(name) {}

        void Initialise() {

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

            if (world.objects.size() == 0) {
                std::cout << "No objects in scene.\n";
            } else {
                for (const auto& object : world.objects) {
                    std::cout << object -> name << '\n';
                }
            }

            std::cout << '\n';
        }

        void EditHelp() {
            std::ostringstream oss;
            int width {40}; 

            int indent {3};

            oss << '\n' <<  std::string(indent, ' ') << "edit commands:\n"
            << std::left << std::string(indent, ' ') << std::setw(width) << "    new <object_type> <object_name>" << "create a new object\n"
            << std::left << std::string(indent, ' ') << std::setw(width) << "    <object_name> <variable> <value>" << "edit a created object\n"
            << std::left << std::string(indent, ' ') << std::setw(width) << "    show <object_name>" << "display object variables\n"
            << std::left << std::string(indent, ' ') << std::setw(width) << "    delete <object_name>" << "delete an object\n"
            << std::left << std::string(indent, ' ') << std::setw(width) << "    quit" << "exit scene editing\n\n";

            std::string output = oss.str();
            std::cout << std::string(indent, ' ') << output;
        }
        
        void AddNewObject(const std::vector<std::string>& object_info) {

            if (object_info[0] == "sphere") {
                world.Add(make_shared<Sphere>(object_info[1], Point3(0, 0, -1), 0.5));
            }
        }

        void ShowObject(const std::string& object_name) {
            for (const auto& object : world.objects) {
                if (object -> name == object_name) {
                    std::ostringstream oss;
                    int width {10}; 
                    int indent {3};

                    std::cout << '\n';

                    std::string prefix = std::string(indent, ' ');
                    if (auto sphere = dynamic_cast<Sphere*>(object.get())) {
                        oss << std::left
                        << std::setw(width) << "object_type: " << "sphere\n"
                        << prefix <<  std::setw(width) << "object_name: " << sphere -> name << "\n"
                        << prefix << std::setw(width) << "center: " << sphere -> center << "\n"
                        << prefix << std::setw(width) << "radius: " << sphere -> radius << "\n\n";

                    }

                    std::string output = oss.str();
                    std::cout << std::string(indent, ' ') << output; 
                }   
            }
        }
};

#endif