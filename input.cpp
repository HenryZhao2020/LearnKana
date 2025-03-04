#include "input.h"
#include "strutil.h"

std::string input::read_line(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    getline(std::cin, line);
    return strutil::strip(line);
}

bool input::read_yn(const std::string& prompt) {
    std::string line = strutil::tolower(read_line(prompt));
    while (line != "y" && line != "n") {
        std::cerr << "Please enter a 'Y' or 'n'! (case-insensitive)!\n";
        line = strutil::tolower(read_line(prompt));
    }
    return line == "y";
}

int input::read_int(const std::string& prompt, int low, int high) {
    while (true) {
        std::string line = read_line(prompt);
        int num;
        
        try {
            num = stoi(line);
        } catch (...) {
            std::cerr << "Please enter a valid integer!\n";
            continue;
        }

        if (num >= low && num <= high) {
            return num;
        }
        std::cerr << "Please enter an integer from " << low << " to " << high << "!\n";
    }
}
