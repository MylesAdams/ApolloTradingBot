//file: gui_test_driver.cpp
//written by ANDREW LAUX

#include"exchange_accnt.h"
#include"kucoin_accnt.h"
#include"gdax_accnt.h"
#include"binance_accnt.h"
#include"log_in.h"
#include<iostream>
#include<string>

//Namespace
using std::cout;
using std::cin;
using std::endl;
using std::string;

//Test
int main() {
	
	//Create a LogIn obj.
	LogIn some_credentials;
	cout << "Creating LogIn object" << endl;

	//Check completeness.
	cout << "Is complete? " << some_credentials.isComplete() << endl << endl;

	//Fill in username.
	string user_entry;
	cout << "Enter username: ";
	cin >> user_entry;
	cin.get();
	cout << endl;

	//Fill in pass.
	string pass_entry;
	cout << "Enter password: ";
	cin >> pass_entry;
	cin.get();
	cout << endl;

	//Check completeness.
	cout << "Is complete? " << some_credentials.isComplete() << endl << endl;

	//Fill in Key.
	string key_entry;
	cout << "Enter API key: ";
	cin >> key_entry;
	cin.get();
	cout << endl;

	//Set LogIn info.
	some_credentials.setLogIn(user_entry, pass_entry, key_entry);

	//Check completeness.
	cout << "Is complete? " << some_credentials.isComplete() << endl << endl;

	//Test copy constructor.
	cout << "Creating copy." << endl << endl;
	LogIn copy(some_credentials);

	//Check completeness.
	cout << "Is copy complete? " << copy.isComplete() << endl << endl;

	//End LogIn tests.
	cout << "End of testing LogIn" << endl << endl;

	//Create a GdaxAccnt obj.
	cout << "----------------------------" << endl;
	cout << "Creating a new Gdax account." << endl << endl;
	GdaxAccnt some_gdax_accnt;
	cout << "Has  id: " << some_gdax_accnt.id << endl;
	cout << "Has url: " << some_gdax_accnt.url << endl << endl;
	cout << "Is exchange active? " << some_gdax_accnt.isActive() << endl << endl;


	//ExitProgram.
	cout << "Test complete . . .";
	cin.get();
	return 0;
}