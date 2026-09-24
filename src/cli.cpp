#include <iostream>
// #include <sstream>

#include "scene_data.hpp"
#include "cli.hpp"
#include "utils.hpp"


void CLI::RunCLI(SceneData& scenes) {

    bool running = true;
    while (running) {
        std::cout << "> ";

        std::string user_input;
        std::getline(std::cin, user_input);

        if (user_input.empty()) {
            continue;
        } 

        if ( user_input == "quit") {
            running = false;
        } 
        else if (user_input == "help") {
            Help();
        } 
        else if (user_input.starts_with("new ")) {
            std::string name = user_input.substr(4);

            auto valid_name = IsValidNameForScene(name);
            if (valid_name != ErrorType::Null) {
                WriteError(valid_name);
                continue;
            }

            if (scenes.HasScene(name)) {
                WriteError(ErrorType::SceneAlreadyExists);
                continue;
            }

            Scene new_scene(name);
            auto create_file_result = FileManager::CreateScene(new_scene);

            if (!create_file_result.HasValue()) {
                WriteError(create_file_result.Error());
                continue;
            }

            scenes.scenes.push_back(new_scene);
            std::cout << '\n' << name << " successfully create.\n\n";
        } 
        else if (user_input.starts_with("edit ")){
            std::string name = user_input.substr(5);

            if (name.empty()) {
                WriteError(ErrorType::FilenameEmpty);
                continue;
            }

            auto result = scenes.FindScene(name);

            if (!result.HasValue()) {
                WriteError(result.Error());
                continue;
            }

            Scene& scene_edit = *result.Value();
            EditScene(scene_edit);
            // Then save changes to file!
            FileManager::SaveScene(scene_edit);
        } 
        else if (user_input == "list") {
            std::vector<Scene> all_scenes = scenes.GetScenes();

            if (all_scenes.size() == 0) {
                std::cout << "\nNo scenes saved.\n\n";
                continue;
            }

            std::cout << '\n';
            for (Scene sc : all_scenes) {
                std::cout << sc.scene_name << '\n';
            }
            std::cout << '\n';
        } 
        else if (user_input.starts_with("show ")) {
            std::string name = user_input.substr(5);

            auto result = scenes.FindScene(name);

            if (!result.HasValue()) {
                WriteError(result.Error());
                continue;
            }

            Scene& scene_show = *result.Value();

            scene_show.Show();
        } 
        else if (user_input.starts_with("render ")) {
            std::string name = user_input.substr(7);

            if (name.empty()) {
                WriteError(ErrorType::FilenameEmpty);
                continue;
            }

            auto result = scenes.FindScene(name);

            if (!result.HasValue()) {
                WriteError(result.Error());
                continue;
            }

            Scene& scene_render = *result.Value();
            scene_render.Initialise();

            auto render_result = FileManager::WriteRender(scene_render);
            
            if (!render_result.HasValue()) {
                WriteError(render_result.Error());
                continue;
            }

            std::cout<< '\n' << name << " successfully rendered.\n";
            std::cout << "find the .ppm file at " << render_result.Value() << "\n\n";
        } 
        else if (user_input.starts_with("delete ")) {
            std::string name = user_input.substr(7);

            if (name.empty()) {
                WriteError(ErrorType::FilenameEmpty);
                continue;
            }

            auto result = scenes.FindScene(name);

            if (!result.HasValue()) {
                WriteError(result.Error());
                continue;
            }

            auto delete_scene_result = scenes.DeleteScene(name);
            auto delete_file_result = FileManager::DeleteScene(name);

            if (!delete_scene_result.HasValue()) {
                WriteError(delete_scene_result.Error());
                continue;
            }
            else if (!delete_file_result.HasValue()) {
                WriteError(delete_file_result.Error());
                continue;
            }

            std::cout << '\n' << name << " successfully deleted.\n\n";
        } 
        else {
            WriteError(ErrorType::UnknownCommand);
        }
    }
}

void CLI::Help() {
    std::ostringstream oss;
    int width {20};

    oss << "\ncommands:\n" << std::left
    << std::setw(width) << "   new <name>" << "create a new scene\n"
    << std::setw(width) << "   edit <name>" << "edit a scene\n"
    << std::setw(width) << "   list" << "list scenes\n"
    << std::setw(width) << "   show <name>" << "display scene contents\n"
    << std::setw(width) << "   render <name>" << "render a scene\n"
    << std::setw(width) << "   delete <name>" << "delete a scene\n"
    << std::setw(width) << "   quit" << "exit program\n\n";

    std::string output = oss.str();
    std::cout << output;
}

void CLI::EditScene(Scene& scene) {
    int indent {3};
    const std::string prefix = std::string(indent, ' ');

    std::cout << '\n';

    bool editing = true;
    while (editing) {
        std::cout << prefix << scene.scene_name << " > ";

        std::string user_input;
        std::getline(std::cin, user_input);

        if (user_input.empty()) {
            continue;
        }

        if (user_input == "quit") {
            std::cout << "\nNo longer editing " << scene.scene_name << ".\n\n";
            editing = false;
        }
        else if (user_input == "help") {
            EditHelp();
        }
        else if (user_input.starts_with("new ")) {
            std::string obj_info = user_input.substr(4);

            if (obj_info.empty()) {
                WriteError(ErrorType::FilenameEmpty, true);
            }

            std::istringstream iss(obj_info);
            std::string object_type;
            std::string object_name;

            iss >> object_type >> object_name;

            if (object_type.empty() || object_name.empty()) {
                WriteError(ErrorType::InvalidObjectName, true);
                continue;
            }

            auto edit_result = scene.AddNewObject(object_type, object_name);

            if (!edit_result.HasValue()) {
                WriteError(edit_result.Error(), true);
                continue;
            }

            std::cout << '\n' << prefix << object_name << " successfully added to " << scene.scene_name << "\n\n";

        }
        else {
            WriteError(ErrorType::UnknownCommand, true);
        }

    }
}

void CLI::EditHelp() {
    int indent {3};
    const std::string prefix = std::string(indent, ' ');

    std::ostringstream oss;
    int width {40};

    oss << '\n' << prefix << "commands:\n" << std::left
    << prefix << std::setw(width) << "   new <object_type> <object_name>" << "create a new object\n"
    << prefix << std::setw(width) << "   <object_name> <object_variable> <value>" << "edit an object\n"
    << prefix << std::setw(width) << "   list" << "list objects and its values\n"
    << prefix << std::setw(width) << "   show <object_name>" << "list an object and its values\n"
    << prefix << std::setw(width) << "   delete <object_name>" << "delete an object\n"
    << prefix << std::setw(width) << "   quit" << "exit scene editing\n\n";

    std::string output = oss.str();
    std::cout << output;
}


