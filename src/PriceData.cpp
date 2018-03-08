#include "PriceData.h"

#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include "Comment.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

Apollo::Bot::PriceData::PriceData()
{
    this->highest_timestamp_seen_ =  "0";

    // Setup base url for cryptocompare's minutely OHLCV API
    Apollo::Bot::PriceDataTarget target(utility::conversions::to_string_t(this->RESOUCE_URL_), utility::conversions::to_string_t(this->DEFAULT_REQUEST_PATH_), web::http::methods::GET);
    this->price_data_target_ = target;

    // Set request to default 12 hour average for BTC -> USD
    setupRequest(DEFAULT_TICKER_FROM_, DEFAULT_TICKER_TO_, DEFAULT_TIME_IN_HOURS);
    updateFullRequestPath();

}

Apollo::Bot::PriceData::~PriceData()
{
}

void Apollo::Bot::PriceData::saveSettings()
{
    std::cout << "Hello" << std::endl;
}

std::string Apollo::Bot::PriceData::requestResponse(const ScraperTarget &target)
{
    return "";
}

std::vector<Apollo::Comment> Apollo::Bot::PriceData::parseJSON(const rapidjson::Document& document)
{
    std::vector<Apollo::Comment> vec;
    return vec;
}

std::vector<Apollo::Comment> Apollo::Bot::PriceData::cleanComments(std::vector<Comment>& comments)
{
    std::vector<Apollo::Comment> vec;
    return vec;
}


uint64_t Apollo::Bot::PriceData::getTime()
{
    return (utility::datetime::utc_timestamp());
}

std::vector<Apollo::Comment> Apollo::Bot::PriceData::getData()
{
    std::vector<Apollo::Comment> vec;
    return vec;
}

void Apollo::Bot::PriceData::setSearchQuery(const std::string &query)
{
}

void Apollo::Bot::PriceData::setTickerFrom(utility::string_t ticker_from, bool update_uri = true)
{
    for (RequestParameter param : this->price_data_target_.request_parameters)
    {
        if (param.key == U("fsym"))
        {
            param.value = utility::conversions::to_utf8string(ticker_from);
            return;
        }
    }
    this->price_data_target_.request_parameters.push_back(RequestParameter(U("fsym"), utility::conversions::to_utf8string(ticker_from)));

    if (update_uri)
        updateFullRequestPath();
}

void Apollo::Bot::PriceData::setTickerTo(utility::string_t ticker_to, bool update_uri = true)
{
    for (RequestParameter param : this->price_data_target_.request_parameters)
    {
        if (param.key == U("tsym"))
        {
            param.value = utility::conversions::to_utf8string(ticker_to);
            return;
        }
    }
    this->price_data_target_.request_parameters.push_back(RequestParameter(U("tsym"), utility::conversions::to_utf8string(ticker_to)));

    if (update_uri)
        updateFullRequestPath();
}

void Apollo::Bot::PriceData::setAveragePeriod(int time_in_hours, bool update_uri = true)
{
    for (auto param : this->price_data_target_.request_parameters)
    {
        if (param.key == U("limit"))
        {
            param.value = utility::conversions::to_string_t((time_in_hours * 60) + "");
            return;
        }
    }
    this->price_data_target_.request_parameters.push_back(RequestParameter(U("limit"), utility::conversions::to_utf8string(std::to_string(time_in_hours * 60))));

    if (update_uri)
        updateFullRequestPath();
}

void Apollo::Bot::PriceData::setupRequest(std::string ticker_from, std::string ticker_to, int time_in_hours)
{
    setTickerFrom(ticker_from, false);
    setTickerTo(ticker_to, false);
    setAveragePeriod(time_in_hours, false);
    updateFullRequestPath();
}

void Apollo::Bot::PriceData::updateHighestTimestampSeen()
{
    time_t temp = utility::datetime::utc_timestamp();
    if (temp - std::stoi(this->highest_timestamp_seen_)>= 60)
        this->highest_timestamp_seen_ = temp - (temp % 60) + "";
}

void Apollo::Bot::PriceData::updateFullRequestPath()
{
    uri_builder builder;
    builder.set_path(this->price_data_target_.request_path);
    for (auto param : this->price_data_target_.request_parameters)
        builder.append_query(utility::conversions::to_utf8string(param.key), utility::conversions::to_utf8string(param.value));

    this->full_request_path_ = builder.to_string();
}

std::string Apollo::Bot::PriceData::requestPriceData()
{
    // Namespace.
    using namespace utility;                    // Common utilities like string conversions.
    using namespace web;                        // Common features like URIs.
    using namespace web::http;                  // Common HTTP functionality.
    using namespace web::http::client;          // HTTP client features.
    using namespace concurrency::streams;		// Asynchronous streams.

    const utility::string_t METHOD(price_data_target_.request_method);
    const utility::string_t CONTENT_TYPE(U("application/json"));

    std::string response;

    http_client client(this->RESOUCE_URL_);

    http_request req(METHOD);
    req.headers().set_content_type(CONTENT_TYPE);
    req.set_request_uri(this->full_request_path_);

    client.request(req).then([](http_response res)
                             {
                                 return res.extract_utf8string();
                             }).then([&response](pplx::task<std::string> utf8str)
                                     {
                                         response = utf8str.get();
                                     }).wait();

    return response;

}






