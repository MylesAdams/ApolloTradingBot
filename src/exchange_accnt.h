//file: exchange_accnt.h
//written by ANDREW LAUX

#ifndef exchange_accnt_h
#define exchange_accnt_h
#include"api_key.h"
#include<string>

//Base class: ExchangeAccnt (virtual)
class ExchangeAccnt {
public:
	std::string url;
	std::string id;
	ApiKey key;
public:
	ExchangeAccnt() :id(""), url(""), key() {}
	ExchangeAccnt(std::string id, std::string url) :id(id), url(url) {}
	bool isActive() { return this->key.isComplete(); }
	virtual void connect() {}
};

#endif // !exchange_accnt.h
