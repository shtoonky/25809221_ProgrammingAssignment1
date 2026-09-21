// #include <iostream>
// #include <string>
#include <iomanip>
#include <sstream>

#include "utils.hpp"

void Help() {
    std::ostringstream oss;
    int width {20};

    oss << "\ncommands:\n"
    << std::left << std::setw(width) << "   new <name>" << "create a new scene\n"
    << std::left << std::setw(width) << "   edit <name>" << "edit a scene\n"
    << std::left << std::setw(width) << "   list" << "list scenes\n"
    << std::left << std::setw(width) << "   show <name>" << "display scene contents\n"
    << std::left << std::setw(width) << "   render <name>" << "render a scene\n"
    << std::left << std::setw(width) << "   delete <name>" << "delete a scene\n"
    << std::left << std::setw(width) << "   quit" << "exit program\n\n";

    std::string output = oss.str();
    std::cout << output;
}

void New(const std::string& name) {
    if (name.empty()) {
        WriteError(ErrorType::FilenameEmpty);
    }
    else if (!IsValidFilename(name)) {
        WriteError(ErrorType::FilenameInvalid);
    }
    else {
        std::cout << "\nNew scene created: " << name << "\n\n";
        // Create a new scene bruh
    }
}

int main() {

    bool running = true;
    while (running) {
        std::cout << "> ";

        std::string user_input;
        // std::cin >> user_input;
        std::getline(std::cin, user_input);

        if (user_input.empty()) {
            continue;
        }
        else if (user_input == "quit") {
            running = false;
        }
        else if (user_input == "help") {
            // std::cout << '\n';
            // std::cout << "Helping... !" << '\n';
            Help();
            // std::cout << '\n';
        }
        else if (user_input.starts_with("new ")) {
            std::string name = user_input.substr(4);
            New(name);
        }
        else {
            // std::cout << '\n';
            // std::cout << "error: unknown command." << '\n';
            // std::cout << "see 'help'." << '\n';
            // std::cout << '\n';
            WriteError(ErrorType::UnknownCommand);
        }
    }
    return 0;
}



// WHAT DID WE LEARN? CANT APPLY SWITCH TO STRINGS