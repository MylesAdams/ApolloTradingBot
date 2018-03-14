#include "TradingBot.h"
#include "TimeoutException.h"

Apollo::TradingBot::TradingBot(std::string trading_currency)
	: price_bot_(),
	exchange_(new Apollo::Exchanges::TestExchange(trading_currency)),
	current_trading_session_(nullptr),
	current_interval_average_(-1),
	last_trading_price_(-1),
	highest_timestamp_seen_(0),
	session_started_(false),
	trading_currency_(trading_currency)
{
    price_bot_.setupRequest(utility::conversions::to_string_t(trading_currency), U("BTC"), 24, U("cryptopia"));
	exchange_->setAmountBTC(10);
	exchange_->setAmountCurrency(0);
};

void Apollo::TradingBot::updateAveragePrice()
{
    if (updateHighestTimestampSeen())
	{
		try
		{
			current_interval_average_ = price_bot_.getIntervalAverage();
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}
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

void Apollo::TradingBot::startSession()
{
	while(1)
	{
		try
		{
			// 1
			updateLastTradingPrice();
			break;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	current_trading_session_ = new Apollo::TradingSession();

	current_trading_session_->setStartTime();
	current_trading_session_->setBTCStartAmount(exchange_->amount_btc_);
	current_trading_session_->setCurrencyStartAmount(exchange_->amount_cur_);
	current_trading_session_->setCurrencyTicker(trading_currency_);
	current_trading_session_->setTotalInBTCStart(last_trading_price_);

	session_started_ = true;
}

void Apollo::TradingBot::endSession()
{
	current_trading_session_->setEndTime();
	current_trading_session_->setBTCEndAmount(exchange_->amount_btc_);
	current_trading_session_->setCurrencyEndAmount(exchange_->amount_cur_);
	current_trading_session_->setTotalInBTCEnd(last_trading_price_);

	current_trading_session_->printSessionToFile();

    delete current_trading_session_;
	current_trading_session_ = nullptr;

	session_started_ = false;
}

void Apollo::TradingBot::updateLastTradingPrice()
{
    try
	{
		// 2
        last_trading_price_ = price_bot_.getLastPrice();
    }
    catch (const std::exception& e)
    {
		throw e;
    }
}

void Apollo::TradingBot::makeTrades()
{
	if (session_started_)
	{
		double diff = (last_trading_price_ - current_interval_average_) / current_interval_average_;

		std::cout << "Avg: " << current_interval_average_ << std::endl;
        std::cout << "Last Price: " << last_trading_price_ << std::endl;
		std::cout << "Difference: " << diff << std::endl << std::endl;

		if (diff <= -STOP_LOSS_PERCENT_)
		{
			std::cout << "STOP LOSS" << std::endl << std::endl;
			// Coin is tanking, sell off to prevent more losses
			exchange_->sellCurrency(1, last_trading_price_);
			std::cout << "BTC: " << exchange_->amount_btc_ << std::endl;
			std::cout << "CUR: " << exchange_->amount_cur_ << std::endl << std::endl;
		}
		else if (diff >= PERCENT_FROM_MEAN_)
		{
			std::cout << "SELL" << std::endl << std::endl;
			// Coin is currently overpriced
			exchange_->sellCurrency(.25, last_trading_price_);
			std::cout << "BTC: " << exchange_->amount_btc_ << std::endl;
			std::cout << "CUR: " << exchange_->amount_cur_ << std::endl << std::endl;
		}
		else if (diff <= -PERCENT_FROM_MEAN_)
		{
			std::cout << "BUY" << std::endl;
			// Coin is currently underpriced
			exchange_->buyCurrency(.25, last_trading_price_);
			std::cout << "BTC: " << exchange_->amount_btc_ << std::endl;
			std::cout << "CUR: " << exchange_->amount_cur_ << std::endl << std::endl;
		}
	}

}

void Apollo::TradingBot::updateAndTrade()
{
	try
	{
		updateAveragePrice();
		updateLastTradingPrice();
		makeTrades();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}


}


