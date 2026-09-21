#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <iostream>

// Functions

enum class ErrorType {
    Null,
    UnknownCommand,
    FilenameEmpty,
    FilenameInvalid,
    FilenameNotFound,
    FilenameAlreadyExists,
    ReadingIssue
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
            break;

        case ErrorType::FilenameNotFound:
            std::cout << "filename could not be found.\n";
            break;

        case ErrorType::FilenameAlreadyExists:
            std::cout << "scene name already exists.\n";
            std::cout << "scene names must be unique.\n";
            break;

        case ErrorType::ReadingIssue:
            std::cout << "something went wrong when trying to read a file.\n";
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