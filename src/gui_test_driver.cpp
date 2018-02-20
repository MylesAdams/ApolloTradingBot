//file: gui_test_driver.cpp
//written by ANDREW LAUX

#include"exchange_manager.h"
#include<iostream>
#include<string>

//Namespace
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

//Test
int main() {
	

	//Program Try block.
	try {

		//Initialize ExchangeManager
		ExchangeManager manager;

		//Initialize char to hold selection.
		char selection;

		//Main Loop.
		do {

			//Get Active exchanges.
			vector<std::string> actives = manager.getActive();

			//Message.
			cout << "List of Active Exchanges" << endl;
			cout << "------------------------" << endl;

			//Check if list is empty.
			if (actives.size() == 0) {
				cout << "no exchanges found . . ." << endl << endl;
			}

			//Else print active exchanges.
			int count = 0;
			for (auto& i : actives) {

			}
			cin.get(selection);

		} while (selection != '0');

	}

	//Catch NotFoundException
	catch (NotFoundException ex) {

	}
	

	//ExitProgram.
	cout << "Test complete . . .";
	cin.get();
	return 0;
}