#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <limits>

using namespace std;


inline void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

#endif
