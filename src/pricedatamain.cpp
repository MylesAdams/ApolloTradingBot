
#include <cstdint>
#include <iostream>
#include <string>
#include <cpprest/asyncrt_utils.h>
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include <stdlib.h>
#include <thread>
#include <chrono> 

#include "PriceData.h"
#include "TradingBot.h"

const int TIME_DAY = 86400;


using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;
int main()
{
//    time_t time = utility::datetime::utc_timestamp();
//    time_t subTime = time % TIME_DAY;
//    time = time - subTime;
//    char date[20];
//    struct tm *tm = localtime(&time);
//    strftime(date, sizeof(date), "%Y-%m-%d", tm);
//    std::cout << date << time << std::endl;

//    uri_builder builder("https://min-api.cryptocompare.com/data/histominute?");
//    uri_builder builder;
//    builder.set_scheme("https");
//    builder.set_host("min-api.cryptocompare.com");
//    builder.set_path("/data/histominute");
//    builder.append_query("fsym", "ETH");
//    builder.append_query("tsym", "USD");
//    builder.append_query("limit", "720");


//    ucout << builder.to_string() << std::endl;

 //   Apollo::Bot::PriceData priceBot;
	//priceBot.setupRequest(U("ETH"), U("BTC"), 12);
 //   std::cout << priceBot.full_request_path_interval_ << std::endl;
	//std::cout << priceBot.full_request_path_instant_ << std::endl;
	//std::cout << priceBot.requestLastPrice() << std::endl;

 //   std::cout << priceBot.getIntervalAverage() << std::endl;

	Apollo::TradingBot testBot("ZCL");
	testBot.startSession();

	std::cout << "SESSION STARTED" << std::endl;

	for (int i = 0; i < 9600; i++)
	{
		testBot.updateAndTrade();
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}

	testBot.endSession();

	std::cout << "SESSION ENDED" << std::endl;

    return 0;

}