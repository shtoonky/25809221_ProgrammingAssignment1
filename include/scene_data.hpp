#ifndef SCENE_DATA_HPP
#define SCENE_DATA_HPP

#include "scene.hpp"
#include "file_manager.hpp"

#include <vector>

class SceneData {
    public:
        std::vector<Scene> scenes;

        // Small Helpers
        bool HasScene(const std::string& name) {
            for (auto scene : scenes) {
                if (scene.scene_name == name) {
                    return true;
                }
            }
            return false;
        }

        Result<Scene*, ErrorType> FindScene(const std::string& name) {
            for (auto& scene : scenes) {
                if (scene.scene_name == name) {
                    return Result<Scene*, ErrorType>::Success(&scene);
                }
            }
            return Result<Scene*, ErrorType>::Failure(ErrorType::FilenameNotFound);
        }

        // CLI
        std::vector<Scene> GetScenes() {
            std::vector<Scene> all_scenes;
            for (Scene sc : scenes) {
                all_scenes.push_back(sc);
            }
            return all_scenes;
        }

        Result<void, ErrorType> DeleteScene(const std::string& name) {
            int index {-1};
            for (int i = 0; i < scenes.size(); ++i) {
                if (scenes[i].scene_name == name) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                return Result<void, ErrorType>::Failure(ErrorType::SceneNotFound);
            }

            scenes.erase(scenes.begin() + index);
            return Result<void, ErrorType>::Success();
        }
};

#endif