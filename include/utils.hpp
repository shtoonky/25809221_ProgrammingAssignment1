#ifndef UTILS_HPP
#define UTILS_HPP

#include "file_manager.hpp"
#include "error_type.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

inline void WriteError(ErrorType error, bool editing = false) {
    int indent {3};
    const std::string prefix = editing ? std::string(indent, ' ') : "";

    std::cout << '\n' << prefix << "error: ";

    switch (error) {
        case ErrorType::UnknownCommand:
            std::cout << "unknown command.\n";
            std::cout << prefix << "see 'help'.\n";
            break;
        
        case ErrorType::FilenameEmpty:
            std::cout << "filename cannot be empty.\n";
            break;

        case ErrorType::FilenameInvalid:
            std::cout << "filename is invalid.\n";
            std::cout << prefix << "filenames must not contain special characters: <>:\"/\\|?* \n";
            break;

        case ErrorType::FilenameNotFound:
            std::cout << "filename could not be found.\n";
            break;

        case ErrorType::SceneAlreadyExists:
            std::cout << "scene name already exists.\n";
            std::cout << prefix << "scene names must be unique.\n";
            break;

        case ErrorType::ReadingIssue:
            std::cout << "something went wrong when trying to read a file.\n";
            break;

        case ErrorType::ObjectNameEmpty:
            std::cout << "variable or value given cannot be empty.\n";
            break;  

        case ErrorType::InvalidObjectName:
            std::cout << "object has invalid type or name.\n";
            break;

        case ErrorType::ObjectAlreadyExists:
            std::cout << "object names must be unique.\n";
            break;  

        case ErrorType::ObjectNotFound:
            std::cout << "object could not be found.\n";
            break;  

        case ErrorType::InvalidValue:
            std::cout << "variable or value given is not applicable.\n";
            break;  

        case ErrorType::EmptyValue:
            std::cout << "variable or value given cannot be empty.\n";
            break;  

        case ErrorType::InvalidSceneFile:
            std::cout << "scene file is unable to be rendered.\n";
            break;  

        case ErrorType::InvalidColour:
            std::cout << "colour is invalid.\n";
            std::cout << prefix << "rgb values must be in range [0, 1].\n";
            break;

        case ErrorType::NegativeValue:
            std::cout << "value cannot be less than 0.\n";
            break;
    }
    std::cout << '\n';
}

// Checks if a name for a windows file is valid (mostly).
inline bool IsValidFilename(const std::string& name) {
    if (name.empty()) 
        return false;

    const std::string invalid_chars = "<>:\"/\\|?*";

    for (char c : name) {
        if (invalid_chars.find(c) != std::string::npos) {
            return false;
        }
    }

    if (name.back() == ' ' || name.back() == '.') {
        return false;
    }

    return true;
}

inline ErrorType IsValidNameForScene(const std::string& name) {
    if (name.empty()) {
        return ErrorType::FilenameEmpty;
    }

    if (!IsValidFilename(name)) {
        return ErrorType::FilenameInvalid;
    }

    return ErrorType::Null;
}

inline bool IsObjectName(const std::string& user_input, const std::vector<std::string>& names) {
    for (auto name : names) {
        if (user_input.starts_with(name)) {
            return true;
        } 
    }
    return false;
}

inline bool IsSceneSetting(const std::string& user_input, const std::vector<std::string>& scene_settings) {
    for (auto setting : scene_settings) {
        if (user_input.starts_with(setting + " ")) {
            return true;
        }
    }
    return false;
}

inline bool IsValidVariable(const std::string& user_input) {
    std::vector<std::string> variables {"center","radius"};
    for (auto var : variables) {
        if (user_input == var) {
            return true;
        }
    }
    return false;
}

inline bool IsValidColourValue(const double& x) {
    if (x < 0 || x > 1) {
        return false;
    }

    return true;
}

#endif