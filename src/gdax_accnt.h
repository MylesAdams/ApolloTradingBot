//File: gdax_accnt.h
//Written by ANDREW LAUX

#ifndef gdax_accnt_h
#define gdax_accnt_h
#include"exchange_accnt.h"

//Derived class: GdaxAccnt
class GdaxAccnt : public ExchangeAccnt {

private: // Private data.
    string_t key;
    string_t passphrase;
    string_t secret;

public: // Public methods.
    GdaxAccnt();
    bool hasCredentials() override;
    void connect() override;
    void setCredentials(string_t key, string_t secret, string_t passphrase) override;

private: // Private methods.
    void setCredentials(string_t key, string_t secret) override { assert(0); } // For Kucoin and Binance only.
};

#endif // !gdax_accnt_h