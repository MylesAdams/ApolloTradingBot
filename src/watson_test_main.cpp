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

    // Try block.
    try {

        // Create Watson object.
        Apollo::Watson watson_obj(user, pass);

        // Send watson some empty comments.
        watson_obj.toneToString(U(""));
    }

     // catch apollo::bot::badstatusexception.
    catch(const Apollo::Bot::BadStatusException& e) {

        // what was exception?
        std::cout << e.what() << e.status_code() << std::endl;
    }

    // Catch std::exception.
    catch (const std::exception& e) {

        // What was exception?
        std::cout << e.what() << std::endl;
    }

    //Exit
    ucin.get();
    return 0;
}

#endif