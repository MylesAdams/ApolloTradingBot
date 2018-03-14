#ifndef APOLLOTRADINGBOT_TRADINGBOT_H
#define APOLLOTRADINGBOT_TRADINGBOT_H

#include "PriceData.h"
#include "TradingSession.h"
#include "TestExchange.h"
#include "exchange_accnt.h"

namespace Apollo
{
    class TradingBot
    {
    private:
        const double PERCENT_FROM_MEAN_ = .02;
        const double STOP_LOSS_PERCENT_ = .08;

        Apollo::Bot::PriceData price_bot_;
		Apollo::Exchanges::TestExchange* exchange_;
        Apollo::TradingSession* current_trading_session_;

        // Minute Resolution
        int highest_timestamp_seen_;

		std::string trading_currency_;

		bool session_started_;

    public:
        int current_time_;
        // 12 hour (default) Volume Weighted Average Price
        double current_interval_average_;

        double last_trading_price_;

        TradingBot(std::string trading_currency);
        void updateAveragePrice();
        bool updateHighestTimestampSeen();
        void startSession();
		void endSession();
        void updateLastTradingPrice();
        void makeTrades();

		void updateAndTrade();


    };
}



#endif //APOLLOTRADINGBOT_TRADINGBOT_H
