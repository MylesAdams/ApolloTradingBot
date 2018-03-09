//
// Created by Myles Adams on 3/8/18.
//

#ifndef APOLLOTRADINGBOT_GDAXTESTEXCHANGE_H
#define APOLLOTRADINGBOT_GDAXTESTEXCHANGE_H

#include "exchange_accnt.h"

namespace Apollo {
    namespace Exchanges {
        class GDAXTestExchange : public ExchangeAccnt
        {
        private:
            double amount_btc_;
            double amount_cur_;

        public:
            GDAXTestExchange();
            GDAXTestExchange(std::string trading_currency);
            bool hasCredentials() override;
            void update() override;
            void connect() override;
            void clearCredentials() override;
            void setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase) override;
            void buyCurrency(double amount);
            void sellCurrency(double amount);

        };
    }


}


#endif //APOLLOTRADINGBOT_GDAXTESTEXCHANGE_H
