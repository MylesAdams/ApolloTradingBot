// File: gui_test_driver.cpp
// Written by ANDREW LAUX

#define test_gui 1
#ifdef  test_gui
#include <iostream>
#include "gdax_accnt.h"

// Execute main.
int main() {

    // Declare a pointer to an exchange account.
    ExchangeAccnt* ptr = new Apollo::Exchanges::GdaxAccnt();
    std::cout << "ExchangeAccnt pointer set to a GdaxAccnt object:" << std::endl;

    // Print out exchange info.
    ucout << ptr->id << U(" \"") << ptr->url << U("\"  ");
    if (ptr->isConnected()) ucout << U("Connected\n");
    else ucout << U("Failed\n");

    // Try block.
    try {  ptr->connect(); }

    // Catch std::exception
    catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }

    // Set credentials.
    ucout << U("\nSetting credentials:\n");
    ptr->setCredentials(U("c0d41169560a191c7817c11b6ba4908b"),
        U("+RDw7Q8V9EbHZusYzX3vfub0I80tytDs8RQPd3la8/wpKLShyf+B6113C3xxqiRy0r5c8UWiUmy+xSaATemWIg=="),
        U("mfsacc5sm7"));

    // Try to connect again.
    try { ptr->connect(); }

    // Catch std::exception
    catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }

    // Print out exchange info.
    ucout << ptr->id << U(" \"") << ptr->url << U("\"  ");
    if (ptr->isConnected()) ucout << U("Connected\n");
    else ucout << U("Failed\n");

    // Exit program successfully.
    std::cout << "Press enter to exit program . . .";
    std::cin.get();
    return 0;
}


#endif // test_gui


