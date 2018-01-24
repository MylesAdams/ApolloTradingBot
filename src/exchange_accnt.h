//file: exchange_accnt.h
//written by ANDREW LAUX

#ifndef exchange_accnt_h
#define exchange_accnt_h
#include"log_in.h"
#include<string>

//Base class: ExchangeAccnt (virtual)
class ExchangeAccnt {
public:
	std::string url;
	std::string id;
	LogIn log;
public:
	bool isActive() { return this->log.isComplete(); }
	virtual void connect();
};

//Derived class: KucoinAccnt
class KucoinAccnt : public ExchangeAccnt {
public:
	void connect() override;
};

//Derived class: GdaxAccnt
class GdaxAccnt : public ExchangeAccnt {
public:
	void connect() override;
};

//Derived class: BinanceAccnt
class BinanceAccnt : public ExchangeAccnt {
public:
	void connect() override;
};

#endif // !exchange_accnt.h
