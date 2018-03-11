// File: gui_test_driver.cpp
// Written by ANDREW LAUX

#define test_gui 0
#ifdef  test_gui
#include <iostream>
#include "gdax_accnt.h"
#include "kucoin_accnt.h"

// Execute main.
int main() {

    // Declare a pointer to an exchange account.
    Apollo::Exchanges::ExchangeAccnt* ptr = new Apollo::Exchanges::GdaxAccnt();
    ucout << U("ExchangeAccnt pointer set to a GdaxAccnt object:\n");

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
    if (ptr->isConnected()) ucout << U("Connected\n\n");
    else ucout << U("Failed\n\n");

    // Update GDAX account.
    ucout << U("Updating Gdax account.\n");
    try { ptr->update(); }
    catch (web::json::json_exception e) {
        std::cout << e.what() << std::endl;
    }

    // Print coins from account.
    for (auto i : ptr->coins_vec) {
        ucout << i.coin_id << U(": ");
        ucout << i.amount << U("\n");
    }

    // Creating pointer to KucoinAccnt.
    Apollo::Exchanges::ExchangeAccnt* ptr2 = new Apollo::Exchanges::KucoinAccnt();
    ucout << U("\nCreating pointer to KucoinAccnt object: \n");
    
    // Set credentials.
    ucout << U("Setting credentials.\n");
    ptr2->setCredentials(U("5a9b2d5e4922257665d3dbf8"), U("bcff527d-65fc-4f3e-b823-e2c05c30977c"));

    // Attempt to connect.
    try { ptr2->connect(); }
    catch (std::exception &e) { std::cout << e.what() << std::endl; }

    // Print out exchange info.
    ucout << ptr2->id << U(" \"") << ptr2->url << U("\"  ");
    if (ptr2->isConnected()) ucout << U("Connected\n\n");
    else ucout << U("Failed\n\n");

    // Atempt update.
    try { ptr2->update(); }
    catch (std::exception &e) { e.what(); }

    // Print coins from account.
    for (auto i : ptr2->coins_vec) {
        ucout << i.coin_id << U(": ");
        ucout << i.amount << U("\n");
    }

    try { ptr->buyAtMarket(U("ETH"), U("1")); }
    catch (std::exception &e) { e.what(); }

    try { ptr2->buyAtLimit(U("ETH"), U("1"), U("10")); }
    catch (std::exception &e) { e.what(); }

    // Exit program successfully.
    ucout << U("\n Press enter to exit program . . .");
    ucin.get();
    return 0;
}


#endif // test_gui


