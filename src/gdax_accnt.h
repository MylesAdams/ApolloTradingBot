//File: gdax_accnt.h
//Written by ANDREW LAUX

#ifndef gdax_accnt_h
#define gdax_accnt_h
#include"exchange_accnt.h"

//Derived class: GdaxAccnt
class GdaxAccnt : public ExchangeAccnt {
public:
	GdaxAccnt() :ExchangeAccnt("Gdax", "https://api.gdax.com") {}
	void connect() override;
};

#endif // !gdax_accnt_h