
#include <cstdint>
#include <iostream>
#include <string>
#include <cpprest/asyncrt_utils.h>
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"

#include "PriceData.h"

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

    Apollo::Bot::PriceData priceBot;
    ucout << priceBot.full_request_path_ << std::endl;
//    std::cout << priceBot.requestPriceData() << std::endl;

    std::cout << priceBot.getIntervalAverage() << std::endl;

    return 0;

}