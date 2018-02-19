//File: exchange_manager.h
//Written by ANDREW LAUX

#ifndef exchange_manager_h
#define exchange_manager_h
#include"exchange_accnt.h"
#include"kucoin_accnt.h"
#include"gdax_accnt.h"
#include"binance_accnt.h"
#include"NotFoundException.h"
#include"vector"

//Manager Class: ExchangeManager
class ExchangeManager {
public:
	KucoinAccnt kucoin;
	GdaxAccnt gdax;
	BinanceAccnt binance;
public:
	std::vector<std::string> getInactive();
	std::vector<std::string> getActive();
	ExchangeAccnt* getById(std::string id);
};

#endif // !exchange_manager.h