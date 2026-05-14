#ifndef HELPER_H
#define HELPER_H
#include <iostream>
#include <string>

namespace Helper {
    inline void printHeader(const std::string& title) {
        std::cout << "\n====================================\n";
        std::cout << "  " << title << "\n";
        std::cout << "====================================\n";
    }

    inline void clearScreen() {
        std::cout << "\033[2J\033[1;1H"; 
    }
}

#endif