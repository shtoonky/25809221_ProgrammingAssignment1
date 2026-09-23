#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include "result.hpp"
#include "scene.hpp"
#include "error_type.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

// Handles ALL file i/o
class FileManager {
    public:

        // Prints the name of all files (excluding file extensions) in the Scenes directory.
        static void PrintSceneNames() {
            for (const auto& entry : fs::directory_iterator("Scenes")) {
                std::cout << entry.path().stem().string() << '\n';
            }  
        }

        // Creates a new scene file in the Scenes directory.
        static Result<void, ErrorType> CreateScene(const std::string& name) {
            try {
                fs::path filepath = fs::path("Scenes") / (name + ".txt");

                if (fs::exists(filepath)) {
                    return Result<void, ErrorType>::Failure(ErrorType::SceneAlreadyExists); 
                }
                
                // Open the file
                std::ofstream file(filepath);

                if (!file) {
                    return Result<void, ErrorType>::Failure(ErrorType::FilenameNotFound); 
                }

                // Write default data.
                file << "scene_name=" << name << '\n';
                file << "aspect_ratio=1.0\n";
                file << "image_width=100\n";
                file << "samples_per_pixel=10\n\n";
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
                for (const auto& entry : fs::directory_iterator("Scenes")) {
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
            fs::path filepath = fs::path("Scenes") / (name + ".txt");
            if (fs::exists(filepath)) {
                return true;
            }
            return false;
        }

        // Returns a scene based on the contents of a scene file.
        static Result<Scene, ErrorType> LoadScene(const std::string& name) {

            try {

                Scene scene;
                fs::path filepath;

                // Find the correct scene by name
                for (const auto& entry : fs::directory_iterator("Scenes")) {
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
                        // then read objects ig :|
                    }
                }

                return Result<Scene, ErrorType>::Success(scene);
            }
            catch (const fs::filesystem_error& e) {
                return Result<Scene, ErrorType>::Failure(ErrorType::ReadingIssue);
            }
        }

};

#endif