//File: binance_accnt.h
//Written by ANDREW LAUX

#ifndef binance_accnt_h
#define binance_accnt_h
#include"exchange_accnt.h"

//Derived class: BinanceAccnt
class BinanceAccnt : public ExchangeAccnt {
public:
	BinanceAccnt() :ExchangeAccnt("Binance", "https://api.gdax.com") {}
	void connect() override;
};

#endif // !gdax_accnt_h