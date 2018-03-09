//File: binance_accnt.h
//Written by ANDREW LAUX

#ifndef binance_accnt_h
#define binance_accnt_h
#include"exchange_accnt.h"

// Define namespace Apollo::Exchanges::BinanceAccnt
namespace Apollo {
    namespace Exchanges {

        //Derived class: KucoinAccnt
        class BinanceAccnt : public ExchangeAccnt {

        private: // Private data.
            utility::string_t key;
            utility::string_t secret;

        public: // Public methods.
            BinanceAccnt();
            bool hasCredentials() override;
            void update() override;
            void connect() override;
            void clearCredentials() override;
            void setCredentials(utility::string_t key, utility::string_t secret) override; // Correct overload for Kucoin.

        private: // Private methods.
            void setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase) override { assert(0); } // For GDAX only.

        };
    }
}



#endif // !gdax_accnt_h