#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include "scene.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/colour.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

// Handles ALL file i/o
class FileManager {
    public:
        // Creates a new scene file in the Scenes directory.
        static Result<void, ErrorType> CreateScene(const Scene& scene) {
            try {
                fs::path filepath = fs::path("Data/Scenes") / (scene.scene_name + ".txt");

                if (fs::exists(filepath)) {
                    return Result<void, ErrorType>::Failure(ErrorType::SceneAlreadyExists); 
                }
                
                // Open the file
                std::ofstream file(filepath);

                if (!file) {
                    return Result<void, ErrorType>::Failure(ErrorType::FilenameNotFound); 
                }

                // Write default data.
                file << "scene_name=" << scene.scene_name << '\n';
                file << "aspect_ratio=" << scene.aspect_ratio << '\n';
                file << "image_width=" << scene.image_width << '\n';
                file << "samples_per_pixel=" << scene.samples_per_pixel << "\n\n";
                file << "[Objects]\n";

                return Result<void, ErrorType>::Success();
            }
            catch (const fs::filesystem_error& e) {
                return Result<void, ErrorType>::Failure(ErrorType::ReadingIssue); 
            } 
        }

        // Deletes a scene file in the Scene directory.
        static Result<void, ErrorType> DeleteScene(const std::string& name) {
            try {
                for (const auto& entry : fs::directory_iterator("Data/Scenes")) {
                    if (entry.path().stem().string() == name) {
                        fs::remove(entry.path());
                        return Result<void, ErrorType>::Success();
                    }
                }  
                return Result<void, ErrorType>::Failure(ErrorType::FilenameNotFound);
            }
            catch (const fs::filesystem_error& e) {
                return Result<void, ErrorType>::Failure(ErrorType::ReadingIssue);
            }
        }

        // Checks if a scene file (<scene_name>.txt) exists in the Scene directory.
        static bool SceneExists(const std::string& name) {
            fs::path filepath = fs::path("Data/Scenes") / (name + ".txt");
            if (fs::exists(filepath)) {
                return true;
            }
            return false;
        }

        static Result<std::vector<Scene>, ErrorType> LoadScenes() {
            std::vector<Scene> scenes;

            for (const auto& entry : fs::directory_iterator("Data/Scenes")) {
                std::string name = entry.path().stem().string();
                Result<Scene, ErrorType> scene = LoadScene(name);

                if (!scene.HasValue()) {
                    continue;
                }
                scenes.push_back(scene.Value());
            } 
            return Result<std::vector<Scene>, ErrorType>::Success(scenes);
        }

        // Returns a scene based on the contents of a scene file.
        static Result<Scene, ErrorType> LoadScene(const std::string& name) {

            try {

                Scene scene;
                fs::path filepath;

                // Find the correct scene by name
                for (const auto& entry : fs::directory_iterator("Data/Scenes")) {
                    if (entry.path().stem().string() == name) {
                        filepath = entry.path();
                    }
                }
                
                // If the filepath is default constructed, return an error.
                if (filepath.empty()) {
                    return Result<Scene, ErrorType>::Failure(ErrorType::SceneNotFound);
                }

                std::ifstream file(filepath);
                std::string line;

                bool reading_objects = false;
                int current_obj_index {0};

                while (std::getline(file, line)) {
                    auto separator = line.find('='); //std::size_t

                    if (line == "[Objects]") {
                        reading_objects = true;
                        continue;
                    }

                    if (!reading_objects) {
                        if (separator == std::string::npos) {
                            continue;
                        }
                        std::string key = line.substr(0, separator);
                        std::string value = line.substr(separator + 1);                         

                        if (key == "scene_name") {
                            scene.scene_name = value;
                        }
                        else if (key == "aspect_ratio") {
                            scene.aspect_ratio = std::stod(value);
                        }
                        else if (key == "image_width") {
                            scene.image_width = std::stoi(value);
                        }
                        else if (key == "samples_per_pixel") {
                            scene.samples_per_pixel = std::stoi(value);
                        }                        
                    }
                    else {
                        std::vector<std::string> object_data;
                        std::stringstream ss(line);
                        std::string value;

                        while (std::getline(ss, value, ',')) {
                            object_data.push_back(value);
                        }

                        if (object_data[0] == "sphere") {
                            auto name = object_data[1];
                            auto center = Point3(std::stod(object_data[2]), std::stod(object_data[3]), std::stod(object_data[4]));
                            double radius = std::stod(object_data[5]);
                            scene.world.Add(make_shared<Sphere>(name, center, radius));
                        }
                    }
                }

                file.close();
                return Result<Scene, ErrorType>::Success(scene);
            }
            catch (const fs::filesystem_error& e) {
                return Result<Scene, ErrorType>::Failure(ErrorType::ReadingIssue);
            }
        }

        // Creates a ppm file based on scene data.
        static Result<std::string, ErrorType> WriteRender(const Scene& scene) {
            std::ofstream output_file("Data/Renders/" + scene.scene_name + ".ppm");
            std::string output_dir("Data/Renders/" + scene.scene_name + ".ppm");

            if (!output_file.is_open()) { // Error opening the file.
                return Result<std::string, ErrorType>::Failure(ErrorType::ReadingIssue);
            }

            const Camera& cam = scene.cam;

            output_file << "P3\n" << scene.image_width << ' ' << cam.GetImageHeight() << "\n255\n";

            for (int j = 0; j < cam.GetImageHeight(); j++) {

                std::clog << "\rScanlines remaining: " << (cam.GetImageHeight() - j) << ' ' << std::flush;

                for (int i = 0; i < scene.image_width; i++) {
                    Colour pixel_colour = Colour (0, 0, 0);
                    if (cam.samples_per_pixel == 0) {  
                        auto pixel_center = cam.GetPixel00() + (i * cam.GetPixelDeltaU() + (j * cam.GetPixelDeltaV()));
                        auto ray_direction = pixel_center - cam.GetCenter();
                        Ray r(cam.GetCenter(), ray_direction);

                        pixel_colour = cam.RayColour(r, scene.world);
                        WriteColour(output_file, pixel_colour);
                    }
                    else {
                        pixel_colour = Colour(0, 0, 0);

                        for (int sample = 0; sample < cam.samples_per_pixel; sample++) {
                            Ray r = cam.GetRay(i, j);
                            pixel_colour += cam.RayColour(r, scene.world);
                        }
                        WriteColour(output_file, pixel_colour * cam.GetPixelSamplesScale());
                    }
                }
            }
            std::clog << "\r                             ";
            return Result<std::string, ErrorType>::Success(output_dir);
        }

        // Write a scene's contents to memory.
        static Result<void, ErrorType> SaveScene(const Scene& scene) {
            std::ofstream output_file("Data/Scenes/" + scene.scene_name + ".txt");

            if (!output_file.is_open()) {
                return Result<void, ErrorType>::Failure(ErrorType::ReadingIssue);
            }

            output_file << "scene_name=" << scene.scene_name << '\n';
            output_file << "aspect_ratio=" << scene.aspect_ratio << '\n';
            output_file << "image_width=" << scene.image_width << '\n';
            output_file << "samples_per_pixel=" << scene.samples_per_pixel << "\n\n";
            output_file << "[Objects]\n";

            for (const auto& object : scene.world.objects) {
                if (auto sphere = std::dynamic_pointer_cast<Sphere>(object)) {
                    output_file << "sphere," << sphere -> name << ',' << sphere -> center.e[0] << ',' << sphere -> center.e[1] << ',' << sphere -> center.e[2]  << ',' << sphere -> radius << '\n';
                }
                else {
                    continue;
                }
            }
            return Result<void, ErrorType>::Success();
        }
};

#endif