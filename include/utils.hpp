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

        case ErrorType::InvalidObjectName:
            std::cout << "object has invalid type or name.\n";
            break;
    }
    std::cout << '\n';
}

inline ErrorType IsValidScene(const std::string& name) {
    if (name.empty()) {
        return ErrorType::FilenameEmpty;
    }
    
    if (!FileManager::SceneExists(name)) {
        return ErrorType::FilenameNotFound;
    }

    return ErrorType::Null;   
}

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

#endif