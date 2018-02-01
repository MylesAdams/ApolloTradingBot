//FILE: exchange_manager.cpp
//Written by ANDREW LAUX
#include "exchange_manager.h"

//Returns a vector of pointers to the exchanges which lack keys.
std::vector<std::string> ExchangeManager::getInactive() {

	//Create a temp vector.
	std::vector<std::string> vec;

	//Check each exchange, add if empty.
	if (!kucoin.isActive()) vec.push_back(this->kucoin.id);
	if (!gdax.isActive()) vec.push_back(this->gdax.id);
	if (!binance.isActive()) vec.push_back(this->binance.id);

	//Return a copy of the vector
	return vec;
}

//Returns a vector of pointers to the active exchanges.
std::vector<std::string> ExchangeManager::getActive() {

	//Create a temp vector.
	std::vector<std::string> vec;

	//Check each exchange, add if empty.
	if (kucoin.isActive()) vec.push_back(this->kucoin.id);
	if (gdax.isActive()) vec.push_back(this->gdax.id);
	if (binance.isActive()) vec.push_back(this->binance.id);

	//Return a copy of the vector
	return vec;
}

//Rrturns a pointer to exchange with matching id.
ExchangeAccnt* ExchangeManager::getById(std::string id) {

	//Temp ptr.
	ExchangeAccnt* ptr;

	//Match with correct exchange.
	if (id == kucoin.id) ptr = &this->kucoin;
	else if (id == gdax.id) ptr = &this->gdax;
	else if (id == binance.id) ptr = &this->binance;

	//Else throw exception.
	else throw NotFoundException("Exchange with ID: " + id + " not found!");

	//Return copy of ptr.
	return ptr;
}