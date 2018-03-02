//file: exchange_accnt.h
//written by ANDREW LAUX

#ifndef exchange_accnt_h
#define exchange_accnt_h
#include<cpprest/http_client.h>
#include<cpprest/json.h>
#include<openssl/hmac.h>
#include<vector>
#include<assert.h>
#include<exception>

// Define namespace.
using namespace utility;

// Base class: ExchangeAccnt (virtual)
class ExchangeAccnt {

public: // Public data members.
    string_t url;
    string_t id;

protected: // Private data.
    bool connected;

public: // Public methods.
    ExchangeAccnt() : id(U("")), url(U("")), connected(0) {};
    ExchangeAccnt(string_t id, string_t url) : id(id), url(url), connected(0) {}
    inline bool isConnected() { return this->connected; }

protected: // Virtuals private methods.
    virtual bool hasCredentials() = 0;

public: // Virtual public methods.
    virtual void setCredentials(string_t key, string_t secret, string_t passphrase) = 0;
    virtual void setCredentials(string_t key, string_t secret) = 0;
    virtual void connect() = 0;
    virtual ~ExchangeAccnt() {};
    
};

#endif // !exchange_accnt.h
