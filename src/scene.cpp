#include "scene.hpp"
#include "utils.hpp"

#include <sstream>
#include <iomanip>

void Scene::Initialise() {
    cam.aspect_ratio = aspect_ratio;
    cam.image_width = image_width;
    cam.Initialise();
}

void Scene::Show() {
    std::ostringstream oss;
    int width {15};

    oss << "\n"
    << std::left << std::setw(width) << "scene_name: " << scene_name << "\n"
    << std::setw(width) << "aspect_ratio: " << aspect_ratio << "\n"
    << std::setw(width) << "image_width: " << image_width << "\n"
    << std::setw(width) << "anti_aliasing: " << samples_per_pixel << "\n\n";

    std::string output = oss.str();
    std::cout << output;

    if (world.objects.size() == 0) {
        std::cout << "No objects in scene.\n\n";
        return;
    }

    std::cout << "scene_objects:\n";
    for (const auto& object : world.objects) {
        std::cout << object->name << '\n';
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



