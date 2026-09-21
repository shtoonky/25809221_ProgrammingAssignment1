#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include "scene.hpp"
#include <filesystem>

// Handles ALL file i/o
class FileManager {
    public:
        const std::string ScenesFilePath = "Data/";

        // Returns a vector of scenes found in the Scenes folder
        static std::vector<Scene> Load() {
            
        }
};

#endif