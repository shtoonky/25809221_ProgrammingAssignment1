
#include "file_manager.hpp"
#include "cli.hpp"
#include "scene_data.hpp"

// build with: g++ -std=c++20 -Iinclude src/main.cpp src/cli.cpp src/scene.cpp -o 25809221_ProgrammingAssignment1

int main() {
    SceneData data;
    auto result = FileManager::LoadScenes(); // Attempt to load an in-memory database

    if (!result.HasValue()) { 
        return -1; // Exit program if something goes wrong with Loading Scenes
    } else {
        data.scenes = result.Value();
    }

    CLI::RunCLI(data); // Run command line interface

    return 0;   
}
