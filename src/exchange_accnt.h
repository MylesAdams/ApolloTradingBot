//file: exchange_accnt.h
//written by ANDREW LAUX

#ifndef exchange_accnt_h
#define exchange_accnt_h

#include"coin.h"
#include"BadStatusException.h"

#include<cpprest/http_client.h>
#include<cpprest/json.h>
#include<openssl/hmac.h>

#include<vector>
#include<assert.h>
#include<exception>

// Define namespace Apollo::Exchanges::ExchangeAccnt
namespace Apollo {
    namespace Exchanges {

        // Base class: ExchangeAccnt (virtual)
        class ExchangeAccnt {

        public: // Public data members.
            std::vector<Coin> coins_vec;
            utility::string_t url;
            utility::string_t id;

        protected: // Private data.
            bool connected;

        public: // Public methods.
            ExchangeAccnt() : id(U("")), url(U("")), connected(0) {};
            ExchangeAccnt(utility::string_t id, utility::string_t url) : id(id), url(url), connected(0) {}
            inline bool isConnected() { return this->connected; }

        public: // Virtual public methods.
            virtual void setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase) = 0;
            virtual void setCredentials(utility::string_t key, utility::string_t secret) = 0;
            virtual bool hasCredentials() = 0;
            virtual void clearCredentials() = 0;
            virtual void update() = 0;
            virtual void connect() = 0;
            virtual ~ExchangeAccnt() {};

        };
    }
}


#endif // !exchange_accnt.h
