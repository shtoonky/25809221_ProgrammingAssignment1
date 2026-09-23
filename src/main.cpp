#include <iomanip>
#include <sstream>

#include "utils.hpp"
#include "file_manager.hpp"

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

// Creates a scene with default values.
void NewScene(const std::string& name) {
    // Check validity of name.
    if (name.empty()) {
        WriteError(ErrorType::FilenameEmpty);
        return;
    }
    if (!IsValidFilename(name)) {
        WriteError(ErrorType::FilenameInvalid);
        return;
    }

    // Attempt to create a new scene.
    Result<void, ErrorType> result = FileManager::CreateScene(name);

    if (!result.HasValue()) {
        WriteError(result.Error());
    } else {
        std::cout << '\n' << name << " was successfully created.\n\n";
    }
}

// Allows the user to edit a saved scene.
void EditScene(const std::string& name) {
    // Look through saved scenes and find it
    // Otherwise throw an error where there was no scene found
    std::cout << "\nscene editing has not been implemented yet.\n\n";
}

// Look through saved scenes and list all names
void ListScenes() {
    
    std::cout << '\n';
    FileManager::PrintSceneNames();
    std::cout << '\n';
}

// Show the details of a saved scene.
void ShowScene(const std::string& name) {
    ErrorType is_valid = IsValidScene(name);
    if (is_valid != ErrorType::Null) {
        WriteError(is_valid);
        return;
    }

    Scene scene = FileManager::LoadScene(name).Value();

    // Scene scene = FileManager::LoadScene(name);
    // // Need to check if the contents of the scene is valid, or it just spews gobbledegook.

    std::cout << '\n' << "Scene name: " << scene.scene_name << '\n';
    std::cout << "Aspect ratio: " << scene.aspect_ratio << '\n';
    std::cout << "Image width: " << scene.image_width << '\n';
    std::cout << "Anti-aliasing strength: " << scene.samples_per_pixel << "\n\n";

}

// Output a .ppm file to the Renders folder of a saved scene.
void RenderScene(const std::string& name) {
    // Look through saved scenes and find it
    // Otherwise throw an error where there was no scene found
    // A ppm should be generated if found
    std::cout << "\nscene rendering has not been implemented yet.\n\n";
}

// Delete a saved scene permanently.
void DeleteScene(const std::string& name) {
    // Check if scene exists.
    ErrorType is_valid = IsValidScene(name);
    if (is_valid != ErrorType::Null) {
        WriteError(is_valid);
        return;
    }
   
    // Attempt to delete scene.
    Result<void, ErrorType> result = FileManager::DeleteScene(name);
    if (!result.HasValue()) {
        WriteError(result.Error());
    } else {
        std::cout << '\n' << name << " was successfully deleted.\n\n";
    }
}

// Main menu
int main() {

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
            NewScene(name);
        }
        else if (user_input.starts_with("edit ")) {
            std::string name = user_input.substr(4);
            EditScene(name);
        }
        else if (user_input == "list") {
            ListScenes();
        }
        else if (user_input.starts_with("show ")) {
            std::string name = user_input.substr(5);
            ShowScene(name);
        }
        else if (user_input.starts_with("render ")) {
            std::string name = user_input.substr(7);
            RenderScene(name);
        }
        else if (user_input.starts_with("delete ")) {
            std::string name = user_input.substr(7);
            DeleteScene(name);
        }
        else {
            WriteError(ErrorType::UnknownCommand);
        }
    }
    return 0;
}
