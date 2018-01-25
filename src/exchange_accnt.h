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
	LogIn credentials;
public:
	ExchangeAccnt() :id(""), url(""), credentials() {}
	ExchangeAccnt(std::string id, std::string url) :id(id), url(url) {}
	bool isActive() { return this->credentials.isComplete(); }
	virtual void connect() {}
};

#endif // !exchange_accnt.h
