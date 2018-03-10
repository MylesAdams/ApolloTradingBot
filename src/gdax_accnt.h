//File: gdax_accnt.h
//Written by ANDREW LAUX

#ifndef gdax_accnt_h
#define gdax_accnt_h
#include"exchange_accnt.h"

// Define namespace Apollo::Exchanges
namespace Apollo {
    namespace Exchanges {

        //Derived class: GdaxAccnt
        class GdaxAccnt : public ExchangeAccnt {

        private: // Private data.
            utility::string_t key;
            utility::string_t passphrase;
            utility::string_t secret;

        public: // Public methods.
            GdaxAccnt();
            bool hasCredentials() override;
            void update() override;
            void connect() override;
            void clearCredentials() override;
            void setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase) override;
            bool buyAtMarket(utility::string_t coin_id, utility::string_t amount) override;

        private: // Private methods.
            web::http::http_response getResponse(utility::string_t request, utility::string_t method, utility::string_t timestamp, utility::string_t path);
            void setCredentials(utility::string_t key, utility::string_t secret) override { assert(0); } // For Kucoin and Binance only.
        };
    }
}


#endif // !gdax_accnt_h