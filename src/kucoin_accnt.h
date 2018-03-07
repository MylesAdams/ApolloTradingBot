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
            void clearCredentials() override;
            void setCredentials(utility::string_t key, utility::string_t secret) override; // Correct overload for Kucoin.

        private: // Private methods.
            void setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase) override { assert(0); } // For GDAX only.

        };
    }
}

#endif // kucoin_accnt.h
