//File: kucoin_accnt.h
//Written by ANDREW LAUX

#ifndef kucoin_accnt_h
#define kucoin_accnt_h
#include"exchange_accnt.h"

//Derived class: KucoinAccnt
class KucoinAccnt : public ExchangeAccnt {
public:
    KucoinAccnt() :ExchangeAccnt("Kucoin", "https://api.kucoin.com") {}
};

#endif // kucoin_accnt.h
