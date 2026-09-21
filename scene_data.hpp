#ifndef SCENE_DATA_HPP
#define SCENE_DATA_HPP

#include "utils.hpp"
#include "file_manager.hpp"
#include "scene.hpp"

class SceneData {
    // holds a vector of scenes
    // this class also updates the vector when scenes are added, edited, or deleted
    // This class uses filemanager to initally create the vector

    public:
        std::vector<Scene> scenes;

        SceneData () {
            // scenes = FileManager::LoadSavedScenes();
        }

};

#endif