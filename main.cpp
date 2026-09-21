#include <iomanip>
#include <sstream>

#include "utils.hpp"

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

void NewScene(const std::string& name) {
    if (name.empty()) {
        WriteError(ErrorType::FilenameEmpty);
    }
    else if (!IsValidFilename(name)) {
        WriteError(ErrorType::FilenameInvalid);
    }
    else {
        std::cout << "\nNew scene created: " << name << "\n\n";
        // Create a new scene bruh
    }
}

void EditScene(const std::string& name) {
    // Look through saved scenes and find it
    // Otherwise throw an error where there was no scene found
}

void ListScenes() {
    // Look through saved scenes and list all names
}

void ShowScene(const std::string& name) {
    // Look through saved scenes and find it
    // Otherwise throw an error where there was no scene found
}

void RenderScene(const std::string& name) {
    // Look through saved scenes and find it
    // Otherwise throw an error where there was no scene found
    // A ppm should be generated if found
}

void DeleteScene(const std::string& name) {
    // Look through saved scenes and find it
    // Otherwise throw an error where there was no scene found
    // scene should be deleted from current build's data and files
}

int main() {

    // Want to load file and store them in SceneData class or something
    // which holds a list of Scenes
    // The list is them initalised using the FileManager, which handles file i/o

    bool running = true;
    while (running) {
        std::cout << "> ";

        std::string user_input;
        // std::cin >> user_input;
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
        else if (user_input.starts_with("edit")) {
            std::string name = user_input.substr(4);
            EditScene(name);
        }
        else if (user_input == "list") {
            ListScenes();
        }
        else if (user_input.starts_with("show ")) {
            std::string name = user_input.substr(4);
            ShowScene(name);
        }
        else if (user_input.starts_with("render ")) {
            std::string name = user_input.substr(4);
            RenderScene(name);
        }
        else if (user_input.starts_with("delete ")) {
            std::string name = user_input.substr(4);
            DeleteScene(name);
        }
        else {
            WriteError(ErrorType::UnknownCommand);
        }
    }
    return 0;
}
