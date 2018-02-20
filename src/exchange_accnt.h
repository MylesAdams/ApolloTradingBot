//file: exchange_accnt.h
//written by ANDREW LAUX

#ifndef exchange_accnt_h
#define exchange_accnt_h
#include<cpprest\http_client.h>
#include<vector>

// Define namespace.
using namespace utility;

// Base class: ExchangeAccnt (virtual)
class ExchangeAccnt {
public:
	string_t url;
	string_t id;

public:
	ExchangeAccnt() :id(U("")), url(U("")) {}
	ExchangeAccnt(string_t id, string_t url) :id(id), url(url) {}
	virtual bool isActive() {}
	std::vector<
};

#endif // !exchange_accnt.h
