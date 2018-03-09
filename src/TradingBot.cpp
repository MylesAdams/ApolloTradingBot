#include "TradingBot.h"

Apollo::TradingBot::TradingBot()
        : price_bot_(),
          current_trading_session_(nullptr),
          current_interval_average_(-1),
          last_trading_price_(-1),
          highest_timestamp_seen_(0)
{
};

void Apollo::TradingBot::updateAveragePrice()
{
    if (updateHighestTimestampSeen())
        current_interval_average_ = price_bot_.getIntervalAverage();
}

bool Apollo::TradingBot::updateHighestTimestampSeen()
{
    time_t temp = utility::datetime::utc_timestamp();
    if (temp - highest_timestamp_seen_ >= 60)
    {
        highest_timestamp_seen_ = temp - (temp % 60);
        return true;
    }
    return false;
}

void Apollo::TradingBot::updateLastTradingPrice()
{
    last_trading_price_ = price_bot_.getLastPrice();
}

void Apollo::TradingBot::makeTrades()
{
    double diff = (last_trading_price_ - current_interval_average_) / current_interval_average_;

    if (diff <= -STOP_LOSS_PERCENT_)
    {
        // STOP LOSS SELL OFF
    }
    else if (diff >= PERCENT_FROM_MEAN_)
    {
        // SELL
    }
    else if (diff <= -PERCENT_FROM_MEAN_)
    {
        // BUY
    }
}
