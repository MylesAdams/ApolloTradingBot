//
// Created by Myles Adams on 3/8/18.
//

#ifndef APOLLOTRADINGBOT_GDAXTESTEXCHANGE_H
#define APOLLOTRADINGBOT_GDAXTESTEXCHANGE_H

#include "exchange_accnt.h"

namespace Apollo {
    namespace Exchanges {
        class TestExchange : public ExchangeAccnt
        {
        private:
			TestExchange();
			std::string trading_currency_ticker_;


        public:
			double amount_btc_;
			double amount_cur_;
            TestExchange(std::string trading_currency);
			TestExchange(std::string trading_currency, double amount_btc, double amount_cur);

			void setAmountBTC(double amount);
			void setAmountCurrency(double amount);
			void setCurrency(std::string trading_currency);
            void buyCurrency(double amount_percent, double price);
            void sellCurrency(double amount_percent, double price);

			bool hasCredentials() override;
			void update() override;
			void connect() override;
			void clearCredentials() override;
			void setCredentials(utility::string_t key, utility::string_t secret) override;
			void setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase) override;
            bool buyAtMarket(utility::string_t coin_id, utility::string_t amount);
            bool buyAtLimit(utility::string_t coin_id, utility::string_t amount, utility::string_t price);
        };
    }
}


#endif //APOLLOTRADINGBOT_GDAXTESTEXCHANGE_H
