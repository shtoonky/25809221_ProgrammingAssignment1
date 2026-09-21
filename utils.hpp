#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <iostream>

// Functions

enum class ErrorType {
    UnknownCommand,
    FilenameEmpty,
    FilenameInvalid
    // FilenameAlreadyExists
};

inline void WriteError(ErrorType error) {
    std::cout << "\nerror: ";

    switch (error) {
        case ErrorType::UnknownCommand:
            std::cout << "unknown command.\n";
            std::cout << "see 'help'.\n";
            break;
        
        case ErrorType::FilenameEmpty:
            std::cout << "filname cannot be empty.\n";
            break;

        case ErrorType::FilenameInvalid:
            std::cout << "filename is invalid.\n";
            std::cout << "filenames must not contain special characters: <>:\"/\\|?* \n";
    }
    std::cout << '\n';
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

// Other useful headers

#include <fstream>
#include <vector>

#endif