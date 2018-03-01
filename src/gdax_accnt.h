//File: gdax_accnt.h
//Written by ANDREW LAUX

#ifndef gdax_accnt_h
#define gdax_accnt_h
#include"exchange_accnt.h"

//Derived class: GdaxAccnt
class GdaxAccnt : public ExchangeAccnt {

private: // Private data members.
    string_t key;                           // API key tied to account.
    string_t passphrase;                    // Passphrass from Gdax.
    string_t secret;                        // Secreat from Gdax.

private: // Private data members.

public: // Public methods.
    GdaxAccnt();
    bool isActive() override;
    void connect() override;
    
};

#endif // !gdax_accnt_h