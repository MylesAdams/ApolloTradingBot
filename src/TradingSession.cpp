//
// Created by Myles Adams on 3/8/18.
//

#include "TradingSession.h"

#include <iomanip>
#include <iostream>
#include <ostream>
#include <cpprest/asyncrt_utils.h>


void Apollo::TradingSession::setStartTime()
{
	start_time_ = utility::datetime::utc_timestamp();
}

void Apollo::TradingSession::setEndTime()
{
	end_time_ = utility::datetime::utc_timestamp();
}

void Apollo::TradingSession::setCurrencyStartAmount(double amount)
{
	currency_start_amount_ = amount;
}

void Apollo::TradingSession::setCurrencyEndAmount(double amount)
{
	currency_end_amount_ = amount;
}

void Apollo::TradingSession::setBTCStartAmount(double amount)
{
	btc_start_amount_ = amount;
}

void Apollo::TradingSession::setBTCEndAmount(double amount)
{
	btc_end_amount_ = amount;
}

void Apollo::TradingSession::setTotalInBTCStart(double first_price)
{
	total_in_btc_start_ = btc_start_amount_ + (currency_start_amount_ * first_price);
}

void Apollo::TradingSession::setTotalInBTCEnd(double last_price)
{
	total_in_btc_end_ = btc_end_amount_ + (currency_end_amount_ * last_price);
}

void Apollo::TradingSession::setCurrencyTicker(std::string ticker)
{
	currency_ticker_ = ticker;
}

void Apollo::TradingSession::printSessionToFile()
{
	//time_t& t = start_time_;
	auto st = *std::localtime(&start_time_);
	auto et = *std::localtime(&end_time_);
	std::ostringstream start_time;
	std::ostringstream end_time;

	start_time << std::put_time(&st, "%m-%d-%Y_%H-%M");
	end_time << std::put_time(&et, "%m-%d-%Y_%H-%M");

	auto file_name = start_time.str() + "_" + currency_ticker_ + "_Session.txt";
	
	std::cout << file_name << std::endl;
	//file_name = "test_session.txt";

	std::ofstream session_file;
	session_file.open("../resources/" + file_name);

	session_file << "Session started at: " << start_time.str() << std::endl;
	session_file << "Session ended at: " << end_time.str() << std::endl;
	session_file << "Currency being traded: " << currency_ticker_ << std::endl << std::endl;

	session_file << "Starting amount of BTC: " << btc_start_amount_ << std::endl;
	session_file << "Starting amount of " << currency_ticker_ << ": " << currency_start_amount_ << std::endl << std::endl;

	session_file << "Ending amount of BTC: " << btc_end_amount_ << std::endl;
	session_file << "Ending amount of " << currency_ticker_ << ": " << currency_end_amount_ << std::endl << std::endl;

	session_file << "Starting total in terms of BTC: " << total_in_btc_start_ << std::endl;
	session_file << "Ending total in terms of BTC: " << total_in_btc_end_ << std::endl << std::endl;

	session_file << "Total Profits/Losses: " << total_in_btc_end_ - total_in_btc_start_ << " BTC" << std::endl;

	session_file.close();
}
