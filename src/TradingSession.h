//
// Created by Myles Adams on 3/8/18.
//

#ifndef APOLLOTRADINGBOT_TRADINGSESSION_H
#define APOLLOTRADINGBOT_TRADINGSESSION_H

#include <string>
#include <cpprest/asyncrt_utils.h>

namespace Apollo{
    class TradingSession
    {
	private:
		time_t start_time_;
		time_t end_time_;

		double currency_start_amount_;
		double currency_end_amount_;

		double btc_start_amount_;
		double btc_end_amount_;

		std::string currency_ticker_;

		double total_in_btc_start_;
		double total_in_btc_end_;
		
		

	public:
		void setStartTime();
		void setEndTime();

		void setCurrencyStartAmount(double amount);
		void setCurrencyEndAmount(double amount);
		void setBTCStartAmount(double amount);
		void setBTCEndAmount(double amount);

		void setTotalInBTCStart(double first_price);
		void setTotalInBTCEnd(double last_price);

		void setCurrencyTicker(std::string ticker);


		void printSessionToFile();
    };
}



#endif //APOLLOTRADINGBOT_TRADINGSESSION_H
