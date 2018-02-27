//File: gdax_accnt.h
//Written by ANDREW LAUX

#ifndef gdax_accnt_h
#define gdax_accnt_h
#include"exchange_accnt.h"

//Derived class: GdaxAccnt
class GdaxAccnt : public ExchangeAccnt {

private: // Private data members.
	string_t key;
	string_t passphrase;
	string_t secret;

public: // Public methods.
	GdaxAccnt() :
		ExchangeAccnt(U("Gdax"), U("https://api.gdax.com")),
		key(U("")),
		passphrase(U("")),
		secret(U("")){}
	bool isActive() override;
};

#endif // !gdax_accnt_h