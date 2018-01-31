//File: exchange_manager.h
//Written by ANDREW LAUX

#ifndef exchange_manager_h
#define exchange_manager_h
#include"exchange_accnt.h"
#include"kucoin_accnt.h"
#include"gdax_accnt.h"
#include"binance_accnt.h"
#include"vector"

//Manager Class: ExchangeManager
class ExchangeManager {
public:
	KucoinAccnt kucoin;
	GdaxAccnt gdax;
	BinanceAccnt binance;
public:
	std::vector<ExchangeAccnt*> getInactive();
	std::vector<ExchangeAccnt*> getActive();
};

#endif // !exchange_manager.h