#include "scene.hpp"
#include "utils.hpp"

#include <sstream>
#include <iomanip>

void Scene::Initialise() {
    cam.aspect_ratio = aspect_ratio;
    cam.image_width = image_width;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth = max_depth;

    cam.skybox_colour_i = skybox_colour_i;
    cam.skybox_colour_j = skybox_colour_j;

    cam.Initialise();
}

void Scene::Show(bool editing) {
    int width {15};
    int indent {3};
    const std::string prefix = (editing) ? std::string(indent, ' ') : "";

    std::cout << "\n"
    << prefix << std::left 
    << std::setw(width) << "scene_name: " << scene_name << "\n"
    << prefix << std::setw(width) << "aspect_ratio: " << aspect_ratio << "\n"
    << prefix << std::setw(width) << "image_width: " << image_width << "\n"
    << prefix << std::setw(width) << "anti_aliasing: " << samples_per_pixel << "\n"
    << prefix << std::setw(width) << "max_depth: " << max_depth << "\n"
    << prefix << std::setw(width) << "skybox_colour_i: " << skybox_colour_i << "\n"
    << prefix << std::setw(width) << "skybox_colour_j: " << skybox_colour_j << "\n\n";

    if (world.objects.size() == 0) {
        std::cout << prefix <<  "No objects in scene.\n\n";
        return;
    }

    std::cout << prefix <<  "scene_objects:\n";
    for (const auto& object : world.objects) {
        std::cout << prefix <<  object->name << '\n';
    }
    std::cout << '\n';
}

bool Scene::HasObject(const std::string& name) {
    for (auto object : world.objects) {
        if (object->name == name) {
            return true;
        }
    }
    return false;
}

Result<shared_ptr<Hittable>, ErrorType> Scene::GetObject(const std::string& name) {
    for (auto object : world.objects) {
        if (object->name == name) {
            return Result<shared_ptr<Hittable>, ErrorType>::Success(object);
        }
    }
    return Result<shared_ptr<Hittable>, ErrorType>::Failure(ErrorType::ObjectNotFound);
}

Result<void, ErrorType> Scene::AddNewObject(const std::string& object_type, const std::string& object_name) {

    if (HasObject(object_name)) {
        return Result<void, ErrorType>::Failure(ErrorType::ObjectAlreadyExists);
    }

    if (object_type == "sphere") {
        world.objects.push_back(make_shared<Sphere>(object_name, Point3(0, 0, -1), 0.5)); // Add sphere with default values
    } 
    else {
        return Result<void, ErrorType>::Failure(ErrorType::InvalidObjectName);
    }
    return Result<void, ErrorType>::Success();
}

Result<void, ErrorType> Scene::RemoveObject(const std::string& name) {

    if (!HasObject(name)) {
        return Result<void, ErrorType>::Failure(ErrorType::ObjectNotFound);
    }

    int index {-1};

    for (int i = 0; i < world.objects.size(); ++i) {
        if (world.objects[i]->name == name) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return Result<void, ErrorType>::Failure(ErrorType::ObjectNotFound);
    }

    world.objects.erase(world.objects.begin() + index);
    return Result<void, ErrorType>::Success();
}

Result<void, ErrorType> Scene::ModifyObject(shared_ptr<Hittable>& object,const std::string& variable,const std::string& value) {
    if (auto sphere = std::dynamic_pointer_cast<Sphere>(object)) {
        if (variable == "radius") {
            sphere->radius = std::stod(value);
        } 
        else if (variable == "center") {
            std::istringstream value_stream(value);
            double x, y, z;

            if (!(value_stream >> x >> y >> z)) {
                return Result<void, ErrorType>::Failure(ErrorType::UnknownCommand);
            }
            sphere->center = Point3(x, y, z);
        }
    } 
    else {
        return Result<void, ErrorType>::Failure(ErrorType::ObjectNotFound);
    }
    return Result<void, ErrorType>::Success();
}

std::vector<std::string> Scene::GetObjectNames() {
    std::vector<std::string> names;

    for (auto& object : world.objects) {
        names.push_back(object->name);
    }

    return names;
}



