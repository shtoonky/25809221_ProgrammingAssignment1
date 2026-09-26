
#include "file_manager.hpp"
#include "cli.hpp"
#include "scene_data.hpp"

// build with: g++ -std=c++20 -Iinclude src/main.cpp src/cli.cpp src/scene.cpp -o program

int main() {
    SceneData data;
    auto result = FileManager::LoadScenes();

    if (!result.HasValue()) {
        return -1;
    } else {
        data.scenes = result.Value();
    }

    CLI::RunCLI(data);

    return 0;   
}
