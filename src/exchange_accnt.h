//file: exchange_accnt.h
//written by ANDREW LAUX

#ifndef exchange_accnt_h
#define exchange_accnt_h
#include<cpprest/http_client.h>
#include<openssl/hmac.h>
#include<vector>

// Define namespace.
using namespace utility;

// Base class: ExchangeAccnt (virtual)
class ExchangeAccnt {

public: // Public data members.
    string_t url;
    string_t id;

protected: // Protected data members.
    bool connected_state;

public: // Public methods.
    ExchangeAccnt() :id(U("")), url(U("")), connected_state(false) {}
    ExchangeAccnt(string_t id, string_t url) :id(id), url(url), connected_state(false) {}
    inline bool isConnected() { return this->connected_state; }

protected: // Virtuals functions.
    virtual bool isActive() {}
    virtual void connect() {}
    
};

#endif // !exchange_accnt.h
