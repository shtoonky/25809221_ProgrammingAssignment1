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
        // User attempts to add a new scene to the program.
        else if (user_input.starts_with("new ")) {
            std::string name = user_input.substr(4);

            // Scene name must not be empty, contain special characters, or be the same as the name for another saved scene.
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
            auto create_file_result = FileManager::SaveScene(new_scene); // Attempt to saved the a default scene to file with the given name

            if (!create_file_result.HasValue()) {
                WriteError(create_file_result.Error());
                continue;
            }

            scenes.scenes.push_back(new_scene); // Only add scene to in-memory database if scene.txt was successfully created
            std::cout << '\n' << name << " successfully created.\n\n";
        } 
        // User edits a saved scene.
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

            FileManager::SaveScene(scene_edit); // Rewrite the scene to disk memory and save changes after user is finished editing
        } 
        // Lists all saved scenes.
        else if (user_input == "list") {
            std::vector<Scene> all_scenes = scenes.GetScenes();

            // Special case for when no scenes are saved.
            if (all_scenes.size() == 0) {
                std::cout << "\nno scenes saved.\n\n";
                continue;
            }

            std::cout << '\n';
            for (Scene sc : all_scenes) {
                std::cout << sc.scene_name << '\n';
            }
            std::cout << '\n';
        } 
        // Shows the properties of a saved scene.
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
        // Render a saved scene to a .ppm file.
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
            scene_render.Initialise(); // Initialise values for Camera object

            auto render_result = FileManager::WriteRender(scene_render); // Write .ppm file to Data/Renders/
            
            if (!render_result.HasValue()) {
                WriteError(render_result.Error());
                continue;
            }

            std::cout<< '\n' << name << " successfully rendered.\n";
            std::cout << "find the .ppm file at " << render_result.Value() << "\n\n"; // WriteRender returns a string with the file directory
        } 
        // User attempts to delete a saved scene.
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

            if (!delete_scene_result.HasValue() || !delete_file_result.HasValue()) {
                WriteError(delete_scene_result.Error());
                continue;
            }

            std::cout << '\n' << name << " successfully deleted.\n\n";
        } 
        // If user_input was not recognised as a command, print an error message.
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

    std::vector<std::string> names = scene.GetObjectNames(); // Used to compare user_input to current object names
    std::vector<std::string> scene_settings {"scene_name","aspect_ratio","image_width","anti_aliasing","max_depth","skybox_colour_i",
        "skybox_colour_j"}; // Used to compare user_input to scene properties

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
        // User attempts to add a new object to scene.
        else if (user_input.starts_with("new ")) {
            std::string obj_info = user_input.substr(4);

            if (obj_info.empty()) {
                WriteError(ErrorType::ObjectNameEmpty, true);
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

            std::cout << '\n' << prefix << object_name << " successfully added to " << scene.scene_name << ".\n\n";
            names = scene.GetObjectNames();
        }
        // User is attempting to modify an object variable.
        else if (IsObjectName(user_input, names)) { 

            std::istringstream iss(user_input);
            std::string object_name;
            std::string variable_name;
            std::string value;

            iss >> object_name >> variable_name;
            std::getline(iss, value); // Some value formatting requires spaces
            value.erase(0, value.find_first_not_of(' '));

            if (variable_name.empty() || value.empty()) {
                WriteError(ErrorType::EmptyValue, true);
                continue;
            }

            if (!IsValidVariable(variable_name)) {
                WriteError(ErrorType::InvalidValue, true);
                continue;
            }
            
            bool modified = false;

            for (auto& object : scene.world.objects) {
                if (object->name == object_name) {
                    auto modify_result = scene.ModifyObject(object, variable_name, value);
                    if (!modify_result.HasValue()) {
                            WriteError(modify_result.Error(), true);
                            break;
                    }

                    modified = true;
                    break;
                }
            }

            if (modified) {
                std::cout << '\n' << prefix << "Successfully modified " << object_name << ".\n\n";
            }
        }
        // User is attempting to modify scene settings.
        else if (IsSceneSetting(user_input, scene_settings)) {
            std::istringstream iss(user_input);
            std::string setting_name;
            std::string value;

            iss >> setting_name;
            std::getline(iss, value);
            value.erase(0, value.find_first_not_of(' '));

            if (setting_name.empty() || value.empty()) {
                WriteError(ErrorType::EmptyValue, true);
                continue;  
            }

            try {
                if (setting_name == "scene_name") {
                    std::string old_name(scene.scene_name);
                    scene.scene_name = value;
                    FileManager::SaveScene(scene);
                    FileManager::DeleteScene(old_name);
                }
                else if (setting_name == "aspect_ratio") {
                    double ratio = std::stod(value);
                    if (ratio < 0) {
                        WriteError(ErrorType::NegativeValue);
                        continue;
                    }
                    scene.aspect_ratio = ratio;
                }
                else if (setting_name == "image_width") {
                    int width = std::stoi(value);
                    if (width < 0) {
                        WriteError(ErrorType::NegativeValue);
                        continue;
                    }
                    scene.image_width = width;
                }
                else if (setting_name == "anti_aliasing") {
                    int aliasing = std::stoi(value);
                    if (aliasing < 0) {
                        WriteError(ErrorType::NegativeValue);
                        continue;
                    }
                    scene.samples_per_pixel = aliasing;
                }
                else if (setting_name == "max_depth") {
                    scene.max_depth = std::stoi(value);
                }
                else if (setting_name == "skybox_colour_i") {
                    std::istringstream value_stream(value);
                    double x, y, z;

                    if (!(value_stream >> x >> y >> z)) {
                        continue;
                    }

                    if (!IsValidColourValue(x) || !IsValidColourValue(y)|| !IsValidColourValue(z)) {
                        WriteError(ErrorType::InvalidColour, true);
                        continue;
                    }

                    scene.skybox_colour_i = Colour(x, y, z);
                }
                else if (setting_name == "skybox_colour_j") {
                    std::istringstream value_stream(value);
                    double x, y, z;

                    if (!(value_stream >> x >> y >> z)) {
                        continue;
                    }

                    if (!IsValidColourValue(x) || !IsValidColourValue(y)|| !IsValidColourValue(z)) {
                        WriteError(ErrorType::InvalidColour, true);
                        continue;
                    }

                    scene.skybox_colour_j = Colour(x, y, z);
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
        // Lists objects in the scene.
        else if (user_input.starts_with("list")) {
            std::cout << '\n';
            // Special case where there are no objects in a scene.
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
        // User attempts to show the properties of an object in the scene.
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
        // User attempts to delete an object in the scene.
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

            std::cout << '\n' << prefix << obj_name << " successfully delete from " << scene.scene_name << ".\n\n";
            names = scene.GetObjectNames();
        }
        // Commands from the initial CLI state, just applied to the current scene.
        else if (user_input == "show") {
            scene.Show(true);
        }
        else if (user_input == "render") {
            scene.Initialise();

            auto render_result = FileManager::WriteRender(scene);
            
            if (!render_result.HasValue()) {
                WriteError(render_result.Error(), true);
                continue;
            }

            std::cout << '\n' << prefix << scene.scene_name << " successfully rendered.\n";
            std::cout << prefix << "find the .ppm file at " << render_result.Value() << "\n\n"; 
        }
        // Specific commands to show the purpose of each scene property.
        else if (user_input == "aspect_ratio") {
            std::cout << '\n' << prefix << "aspect_ratio: proportion between image_width and the rendered image's height.\n\n";
        }
        else if (user_input == "image_width") {
            std::cout << '\n' << prefix << "image_width: width of the rendered image(pixels).\n\n";
        }
        else if (user_input == "anti_aliasing") {
            std::cout << '\n' << prefix << "anti_aliasing: number of rays calculated within one pixel.\n";
            std::cout << '\n' << prefix << "higher anti_aliasing greatly increases render time.\n\n";
        }
        else if (user_input == "max_depth") {
            std::cout << '\n' << prefix << "max_depth: the maximum number of times a ray will bounce between objects.\n";
            std::cout << '\n' << prefix << "higher max+_depth greatly increases render time.\n\n";        
        }
        else if (user_input == "skybox_colour_i") {
            std::cout << '\n' << prefix << "skybox_colour_i: colour at the bottom of the skybox gradient.\n\n";   
        }
        else if (user_input == "skybox_colour_j") {
            std::cout << '\n' << prefix << "skybox_colour_j: colour at the top of the skybox gradient.\n\n";   
        }
        // If user_input was not recognised as a command, print an error message.
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
