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
        // Removes a scene.txt file in the Scene directory.
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

        // Returns a list of Scenes found in the Data/Scenes/ directory.
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

        // Returns a scene based on the contents of a scene.txt file.
        static Result<Scene, ErrorType> LoadScene(const std::string& name) {

            try {
                Scene scene;
                fs::path filepath;

                // Find the correct scene by name.
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
                        else if (key == "max_depth") {
                            scene.max_depth = std::stoi(value);
                        }
                        else if (key == "skybox_colour_i") {
                            std::vector<std::string> colour;
                            std::stringstream ss(value);
                            std::string component;

                            while (std::getline(ss, component, ',')){
                                colour.push_back(component);
                            }

                            scene.skybox_colour_i = Colour(std::stod(colour[0]), std::stod(colour[1]), std::stod(colour[2]));
                        }
                        else if (key == "skybox_colour_j") {
                            std::vector<std::string> colour;
                            std::stringstream ss(value);
                            std::string component;

                            while (std::getline(ss, component, ',')){
                                colour.push_back(component);
                            }

                            scene.skybox_colour_j = Colour(std::stod(colour[0]), std::stod(colour[1]), std::stod(colour[2]));
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
            catch (const std::exception& e) {
                std::cerr << "error loading scene: " << e.what() << '\n';
                return Result<Scene, ErrorType>::Failure(ErrorType::ReadingIssue);
            }
        }

        // Creates a .ppm file based on scene data.
        static Result<std::string, ErrorType> WriteRender(const Scene& scene) {
            std::ofstream output_file("Data/Renders/" + scene.scene_name + ".ppm");
            std::string output_dir("Data/Renders/" + scene.scene_name + ".ppm");

            if (!output_file.is_open()) { // Error opening the file.
                return Result<std::string, ErrorType>::Failure(ErrorType::ReadingIssue);
            }

            const Camera& cam = scene.cam;

            if (cam.image_width <= 0) {
                return Result<std::string, ErrorType>::Failure(ErrorType::InvalidSceneFile);
            }

            cam.Render(output_file, scene.world);

            return Result<std::string, ErrorType>::Success(output_dir);
        }

        // Write a scene's contents to memory.
        static Result<void, ErrorType> SaveScene(const Scene& scene) {
            // Print contents of a Vec3 with commas in between each value.
            auto GetVec3 = [](Vec3 v){
                std::ostringstream out;
                out << v.x() << ',' << v.y() << ',' << v.z();
                return out.str();
            };

            std::ofstream output_file("Data/Scenes/" + scene.scene_name + ".txt");

            if (!output_file.is_open()) {
                return Result<void, ErrorType>::Failure(ErrorType::ReadingIssue);
            }

            output_file << "scene_name=" << scene.scene_name << '\n';
            output_file << "aspect_ratio=" << scene.aspect_ratio << '\n';
            output_file << "image_width=" << scene.image_width << '\n';
            output_file << "samples_per_pixel=" << scene.samples_per_pixel << '\n';
            output_file << "max_depth=" << scene.max_depth << '\n';
            output_file << "skybox_colour_i=" << GetVec3(scene.skybox_colour_i) << '\n';
            output_file << "skybox_colour_j=" << GetVec3(scene.skybox_colour_j) << "\n\n";
            output_file << "[Objects]\n";

            for (const auto& object : scene.world.objects) {
                if (auto sphere = std::dynamic_pointer_cast<Sphere>(object)) {
                    output_file << "sphere," << sphere -> name << ',' << GetVec3(sphere->center) << ',' << sphere -> radius << '\n';
                }
                else {
                    continue;
                }
            }
            return Result<void, ErrorType>::Success();
        }
};

#endif