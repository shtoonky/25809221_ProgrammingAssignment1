#include <iomanip>
#include <sstream>

#include "utils.hpp"
#include "file_manager.hpp"

// build with: g++ -std=c++20 -Iinclude src/main.cpp -o program

class SceneData {
    public:
        std::vector<Scene> scenes;
        
        SceneData() : scenes(FileManager::LoadScenes().Value()) {}

        // Creates a new scene object.
        void NewScene(const std::string& name) {
            scenes.push_back(Scene(name));
        }

        void EditScene(const std::string& name) {
            Scene scene;
            bool found = false; // temp
            for (auto sc : scenes) {
                if (sc.scene_name == name) {
                    scene = sc;
                    found = true;
                    break;
                }
            }
            if (!found) return;

            bool editing = true;
            int indent {3};

            std::cout << '\n';

            while (editing) {
                std::cout << std::string(indent, ' ') << "> ";

                std::string user_input;
                std::getline(std::cin, user_input);

                if (user_input.empty()) {
                    continue;
                } else if (user_input == "quit") {
                    std::cout << "\nNo longer editing.\n\n";
                    editing = false;
                } else if (user_input == "help") {
                    scene.EditHelp();
                } else if (user_input.starts_with("new ")) {
                    std::string obj_info = user_input.substr(4);

                    std::istringstream stream(obj_info);
                    std::string word;
                    std::vector<std::string> words;

                    while (stream >> word) {
                        words.push_back(word);
                    } 

                    scene.AddNewObject(words);
                    // FileManager::AddObjectToScene(scene, words);
                } else if (user_input.starts_with("show ")) {
                    std::string object_name = user_input.substr(5);
                    scene.ShowObject(object_name);

                }
                else {
                    std::cout << std::string(indent, ' '); //<< "Unknown Command. \n\n";
                    WriteError(ErrorType::UnknownCommand, true);
                }
            }
        }

        void ShowScene(const std::string& name) {
            ErrorType is_valid = IsValidScene(name);
            if (is_valid != ErrorType::Null) {
                WriteError(is_valid);
                return;
            }

            for (auto scene : scenes) {
                if (scene.scene_name == name) {
                    scene.ShowScene();
                    return;
                }
            }
            WriteError(ErrorType::FilenameNotFound);
        }

        // Look through saved scenes and list all names.
        void ListScenes() {
            std::cout << '\n';
            for (auto scene : scenes) {
                std::cout << scene.scene_name << '\n';
            }
            std::cout << '\n';
        }

        // Deletes a scene object.
        void DeleteScene(const std::string& name) {
            for (int i = 0; i < scenes.size(); ++i) {
                if (scenes[i].scene_name == name) {
                    scenes.erase(scenes.begin() + i);
                    break;
                }
            }
        }

    private:
        

};

void Help() {
    std::ostringstream oss;
    int width {20};

    oss << "\ncommands:\n"
    << std::left << std::setw(width) << "   new <name>" << "create a new scene\n"
    << std::left << std::setw(width) << "   edit <name>" << "edit a scene\n"
    << std::left << std::setw(width) << "   list" << "list scenes\n"
    << std::left << std::setw(width) << "   show <name>" << "display scene contents\n"
    << std::left << std::setw(width) << "   render <name>" << "render a scene\n"
    << std::left << std::setw(width) << "   delete <name>" << "delete a scene\n"
    << std::left << std::setw(width) << "   quit" << "exit program\n\n";

    std::string output = oss.str();
    std::cout << output;
}

// Creates a scene file with default values.
bool NewScene(const std::string& name) {
    // Check validity of name.
    if (name.empty()) {
        WriteError(ErrorType::FilenameEmpty);
        return false;
    }
    if (!IsValidFilename(name)) {
        WriteError(ErrorType::FilenameInvalid);
        return false;
    }

    // Attempt to create a new scene.
    Result<void, ErrorType> result = FileManager::CreateScene(name);

    if (!result.HasValue()) {
        WriteError(result.Error());
        return false;
    } 

    std::cout << '\n' << name << " was successfully created.\n\n";
    return true;
}

// Allows the user to edit a saved scene.
void EditScene(const std::string& name) {
    ErrorType is_valid = IsValidScene(name);
    if (is_valid != ErrorType::Null) {
        WriteError(is_valid);
        return;
    }
    // Scene scene = FileManager::LoadScene(name).Value();

    /// Edit stuff
}

// Output a .ppm file to the Renders folder of a saved scene.
void RenderScene(const std::string& name) {
    ErrorType is_valid = IsValidScene(name);
    if (is_valid != ErrorType::Null) {
        WriteError(is_valid);
        return;
    }

    Scene scene = FileManager::LoadScene(name).Value();
    scene.Initialise();
    FileManager::WriteRender(scene);
    std::cout << '\n';
}

// Delete a saved scene file permanently.
bool DeleteScene(const std::string& name) {
    // Check if scene exists.
    ErrorType is_valid = IsValidScene(name);
    if (is_valid != ErrorType::Null) {
        WriteError(is_valid);
        return false;
    }

    // Attempt to delete scene.
    Result<void, ErrorType> result = FileManager::DeleteScene(name);
    if (!result.HasValue()) {
        WriteError(result.Error());
        return false;
    } 

    std::cout << '\n' << name << " was successfully deleted.\n\n";
    return true;
}

// Main menu
int main() {

    // Load all scene data
    // When we create a new scene, we add it to the scene data list, as well as a new file
    // When we modify a scene, we do so in the scene data list, and then maybe replace the file contents with the data from the scene?
    auto scenes = SceneData();

    bool running = true;
    while (running) {
        std::cout << "> ";

        std::string user_input;
        std::getline(std::cin, user_input);

        if (user_input.empty()) {
            continue;
        }
        else if (user_input == "quit") {
            running = false;
        }
        else if (user_input == "help") {
            Help();
        }
        else if (user_input.starts_with("new ")) {
            std::string name = user_input.substr(4);
            if (NewScene(name))
                scenes.NewScene(name);
        }
        else if (user_input.starts_with("edit ")) {
            std::string name = user_input.substr(5);
            EditScene(name);
            scenes.EditScene(name);
            scenes = SceneData();
        }
        else if (user_input == "list") {
            scenes.ListScenes();
        }
        else if (user_input.starts_with("show ")) {
            std::string name = user_input.substr(5);
            scenes.ShowScene(name);
        }
        else if (user_input.starts_with("render ")) {
            std::string name = user_input.substr(7);
            RenderScene(name);
        }
        else if (user_input.starts_with("delete ")) {
            std::string name = user_input.substr(7);
            if (DeleteScene(name))
                scenes.DeleteScene(name);
        }
        else {
            WriteError(ErrorType::UnknownCommand);
        }
    }
    return 0;
}
