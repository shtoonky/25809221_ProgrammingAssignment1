#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

// #include "scene.hpp"
#include <filesystem>

namespace fs = std::filesystem;

// Handles ALL file i/o
class FileManager {
    public:

        // Returns a vector of scenes found in the Scenes folder
        // static std::vector<Scene> LoadSavedScenes() {
        //     for (const auto& entry : fs::directory_iterator("Scenes")) {
        //         std::cout << entry.path().stem().string() << '\n';
        //     }
        //     return std::vector<Scene>();
        // }

        // Prints the name of all files (excluding file extensions) in the Scenes directory.
        static void PrintSceneNames() {
            for (const auto& entry : fs::directory_iterator("Scenes")) {
                std::cout << entry.path().stem().string() << '\n';
            }  
        }

        static bool CreateScene(const std::string& name) {
            try {
                fs::path filepath = fs::path("Scenes") / (name + ".txt");

                if (fs::exists(filepath)) {
                    return false; // File already exists in this directory
                }
                
                // Open the file
                std::ofstream file(filepath);

                if (!file) {
                    return false; // No file
                }

                // Write data
                file << "data\n";

                return true;
            }
            catch (const fs::filesystem_error& e) {
                return false; // Something went wrong
            } 
        }

        static bool DeleteScene(const std::string& name) {
            try {
                for (const auto& entry : fs::directory_iterator("Scenes")) {
                    if (entry.path().stem().string() == name) {
                        fs::remove(entry.path());
                        return true; // File was successfully deleted
                    }
                }  
                return false; // File was not found                
            }
            catch (const fs::filesystem_error& e) {
                return false; // Something went wrong
            }
        }
};

#endif