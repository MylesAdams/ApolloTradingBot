//FILE: exchange_manager.cpp
//Written by ANDREW LAUX
#include "exchange_manager.h"

//Returns a vector of pointers to the exchanges which lack keys.
std::vector<ExchangeAccnt*> ExchangeManager::getInactive() {

	//Create a temp vector.
	std::vector<ExchangeAccnt*> vec;

	//Check each exchange, add if empty.
	if (!kucoin.isActive()) vec.push_back(&this->kucoin);
	if (!gdax.isActive()) vec.push_back(&this->gdax);
	if (!binance.isActive()) vec.push_back(&this->binance);

	//Return a copy of the vector
	return vec;
}

//Returns a vector of pointers to the active exchanges.
std::vector<ExchangeAccnt*> ExchangeManager::getActive()
{
	//Create a temp vector.
	std::vector<ExchangeAccnt*> vec;

	//Check each exchange, add if empty.
	if (kucoin.isActive()) vec.push_back(&this->kucoin);
	if (gdax.isActive()) vec.push_back(&this->gdax);
	if (binance.isActive()) vec.push_back(&this->binance);

	//Return a copy of the vector
	return vec;
}
