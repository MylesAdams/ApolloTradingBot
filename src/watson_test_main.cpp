// File: watson_test_main.cpp
// Written by ANDREW LAUX 

#define watson_test   1

#include <iostream>
#include "watson.h"

#if watson_test

// Main
int main() {

    // Declare user name and password variables.
    utility::string_t user;
    utility::string_t pass;

    // Prompt for input.
    ucout << U("Space-serperated user and pass: ");
    ucin >> user;
    ucin >> pass;
    ucin.get();
    ucout << U("\n");

    // Create Watson object.
    Apollo::Watson watson_obj(user, pass);

    //Exit
    ucin.get();
    return 0;
}

#endif