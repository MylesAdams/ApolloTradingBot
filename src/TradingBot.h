#ifndef APOLLOTRADINGBOT_TRADINGBOT_H
#define APOLLOTRADINGBOT_TRADINGBOT_H

#include "PriceData.h"
#include "TradingSession.h"

namespace Apollo
{
    class TradingBot
    {
    private:
        const double PERCENT_FROM_MEAN_ = .05;
        const double STOP_LOSS_PERCENT_ = .15;

        Apollo::Bot::PriceData price_bot_;
        Apollo::TradingSession* current_trading_session_;

        // 12 hour (default) Volume Weighted Average Price
        double current_interval_average_;

        double last_trading_price_;

        // Minute Resolution
        int highest_timestamp_seen_;




    public:
        TradingBot();
        void updateAveragePrice();
        bool updateHighestTimestampSeen();
        void startSession();
        void updateLastTradingPrice();
        void makeTrades();


    };
}



#endif //APOLLOTRADINGBOT_TRADINGBOT_H
