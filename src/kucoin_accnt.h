//File: kucoin_accnt.h
//Written by ANDREW LAUX

#ifndef kucoin_accnt_h
#define kucoin_accnt_h
#include"exchange_accnt.h"

// Define namespace Apollo::Exchanges::KucoinAccnt
namespace Apollo {
    namespace Exchanges {

        //Derived class: KucoinAccnt
        class KucoinAccnt : public ExchangeAccnt {

        private: // Private data.
            utility::string_t key;
            utility::string_t secret;
            int num_pages;

        public: // Public methods.
            KucoinAccnt();
            bool hasCredentials() override;
            void update() override;
            void connect() override;
            bool buy(utility::string_t coin_id, double amount);
            void clearCredentials() override;
            bool buyAtLimit(utility::string_t coin_id, utility::string_t amount, utility::string_t price) override;
            void setCredentials(utility::string_t key, utility::string_t secret) override; // Correct overload for Kucoin.

        private: // Private methods.
            web::http::http_response getResponse(utility::string_t endpoint, utility::string_t nonce, utility::string_t querystring = U(""), utility::string_t method = U("Get"));
            bool buyAtMarket(utility::string_t coin_id, utility::string_t amount) override { assert(0); return 0; } // Not available on Kucoin.
            void setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase) override { assert(0); } // For GDAX only.

        };
    }
}

#endif // kucoin_accnt.h
