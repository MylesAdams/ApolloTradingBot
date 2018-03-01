//File: exchange_manager.h
//Written by ANDREW LAUX

#ifndef exchange_manager_h
#define exchange_manager_h
#include"exchange_accnt.h"
#include"kucoin_accnt.h"
#include"gdax_accnt.h"
#include"binance_accnt.h"

//Manager Class: ExchangeManager
class ExchangeManager {

public: // Singleton data members.
    static KucoinAccnt kucoin;
    static GdaxAccnt gdax;
    static BinanceAccnt binance;

};

#endif // !exchange_manager.h