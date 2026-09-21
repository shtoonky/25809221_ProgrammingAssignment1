#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <filesystem>

namespace fs = std::filesystem;

// Handles ALL file i/o
class FileManager {
    public:

        // Prints the name of all files (excluding file extensions) in the Scenes directory.
        static void PrintSceneNames() {
            for (const auto& entry : fs::directory_iterator("Scenes")) {
                std::cout << entry.path().stem().string() << '\n';
            }  
        }

        static ErrorType CreateScene(const std::string& name) {
            try {
                fs::path filepath = fs::path("Scenes") / (name + ".txt");

                if (fs::exists(filepath)) {
                    return ErrorType::FilenameAlreadyExists;
                }
                
                // Open the file
                std::ofstream file(filepath);

                if (!file) {
                    return ErrorType::FilenameNotFound;
                }

                // Write data
                file << "data\n";

                return ErrorType::Null;
            }
            catch (const fs::filesystem_error& e) {
                return ErrorType::ReadingIssue;
            } 
        }

        static ErrorType DeleteScene(const std::string& name) {
            try {
                for (const auto& entry : fs::directory_iterator("Scenes")) {
                    if (entry.path().stem().string() == name) {
                        fs::remove(entry.path());
                        return ErrorType::Null;
                    }
                }  
                return ErrorType::FilenameNotFound;               
            }
            catch (const fs::filesystem_error& e) {
                return ErrorType::ReadingIssue;
            }
        }
};

#endif