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

    auto names = scene.GetObjectNames();
    std::vector<std::string> scene_settings {"scene_name","aspect_ratio","image_width","antialiasing"};

    std::cout << '\n';

    bool editing = true;
    while (editing) {
        std::cout << prefix << scene.scene_name << " > ";

        std::string user_input;
        std::getline(std::cin, user_input);

        int input_object_name = IsObjectName(user_input, names);

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
                continue;
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
            names = scene.GetObjectNames();
        }
        else if (input_object_name != -1) { // User is attempting to modify an object's value

            std::istringstream iss(user_input);
            std::string object_name;
            std::string variable_name;
            std::string value;

            iss >> object_name >> variable_name;
            std::getline(iss, value);
            value.erase(0, value.find_first_not_of(' '));

            if (variable_name.empty() || value.empty()) {
                WriteError(ErrorType::UnknownCommand, true);
                continue;
            }

            shared_ptr<Hittable> obj;

            for (auto& object : scene.world.objects) {
                if (object->name == object_name) {
                    obj = object;
                    break;
                }
            }
            auto modify_result = scene.ModifyObject(obj, variable_name, value);

            if (!modify_result.HasValue()) {
                WriteError(modify_result.Error(), true);
                continue;
            }

            std::cout << '\n' << prefix << "Successfully modified " << object_name << "\n\n";
        }
        else if (IsSceneSetting(user_input, scene_settings)) { // User is attempting to modify scene settings
            std::istringstream iss(user_input);
            std::string setting_name;
            std::string value;

            iss >> setting_name;
            std::getline(iss, value);
            value.erase(0, value.find_first_not_of(' '));

            if (setting_name.empty() || value.empty()) {
                WriteError(ErrorType::UnknownCommand, true);
                continue;  
            }

            try {
                if (setting_name == "scene_name") {
                    scene.scene_name = value;
                }
                else if (setting_name == "aspect_ratio") {
                    scene.aspect_ratio = std::stod(value);
                }
                else if (setting_name == "image_width") {
                    scene.image_width = std::stoi(value);
                }
                else if (setting_name == "anti_aliasing") {
                    scene.samples_per_pixel = std::stoi(value);
                }
                else {
                    WriteError(ErrorType::UnknownCommand, true);
                    continue;
                }

                std::cout << '\n' << prefix << setting_name << " successfully configured.\n\n";                
            }
            catch (const std::invalid_argument&) {
                WriteError(ErrorType::InvalidValue, true);
                continue;
            }

        }
        else if (user_input.starts_with("list")) {
            std::cout << '\n';

            if (scene.world.objects.size() == 0) {
                std::cout << prefix << "No objects in " << scene.scene_name << ".\n";
            }
            else {
                for (auto& object : scene.world.objects) {
                    if (auto sphere = std::dynamic_pointer_cast<Sphere>(object)) {
                        std::cout << prefix << "sphere " << sphere->name << " (" << sphere->center.e[0] << ", " << sphere->center.e[1] << ", " << sphere->center.e[2] << ") " << sphere->radius << '\n';
                    }
                }                
            }

            std::cout << '\n';
        }
        else if (user_input.starts_with("show ")) {
            std::string obj_name = user_input.substr(5);

            if (obj_name.empty()) {
                WriteError(ErrorType::FilenameEmpty, true);
                continue;
            }

            if (!scene.HasObject(obj_name)) {
                WriteError(ErrorType::ObjectNotFound, true);
                continue;
            }

            auto object_result = scene.GetObject(obj_name);

            if (!object_result.HasValue()) {
                WriteError(object_result.Error(), true);
                continue;
            }

            auto object = object_result.Value();

            ShowObject(object);
        }
        else if (user_input.starts_with("delete ")) {
            std::string obj_name = user_input.substr(7);

            if (obj_name.empty()) {
                WriteError(ErrorType::FilenameEmpty, true);
                continue;
            }

            auto delete_result = scene.RemoveObject(obj_name);

            if (!delete_result.HasValue()) {
                WriteError(delete_result.Error(), true);
                continue;
            }

            std::cout << '\n' << prefix << obj_name << " successfully delete from " << scene.scene_name << "\n\n";
            names = scene.GetObjectNames();
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
    int width {50};

    oss << '\n' << prefix << "commands:\n" << std::left
    << prefix << std::setw(width) << "   new <object_type> <object_name>" << "create a new object\n"
    << prefix << std::setw(width) << "   <object_name> <object_variable> <value>" << "edit an object\n"
    << prefix << std::setw(width) << "   <scene variable> <value>" << "edit scene settings\n"
    << prefix << std::setw(width) << "   list" << "list objects and their values\n"
    << prefix << std::setw(width) << "   show <object_name>" << "list an object and its values\n"
    << prefix << std::setw(width) << "   delete <object_name>" << "delete an object\n"
    << prefix << std::setw(width) << "   quit" << "exit scene editing\n\n"
    << prefix << std::setw(width) << "   view git readme for specific commands for value variance.\n\n";

    std::string output = oss.str();
    std::cout << output;
}

void CLI::ShowObject(const shared_ptr<Hittable>& object) {
    int indent {3};
    const std::string prefix = std::string(indent, ' ');
    if (auto sphere = std::dynamic_pointer_cast<Sphere>(object)) {
        std::cout << '\n' << prefix << "object_type: sphere\n";
        std::cout << prefix << "object_name: " << sphere->name << '\n';
        std::cout << prefix << "center: " << sphere->center << '\n';
        std::cout << prefix << "radius: " << sphere->radius << "\n\n";
    } 
    else {
        WriteError(ErrorType::ObjectNotFound);
    } 
}
