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
    // Setup base url for cryptocompare's minutely OHLCV API
    Apollo::Bot::PriceDataTarget target(utility::conversions::to_string_t(this->RESOUCE_URL_), utility::conversions::to_string_t(this->DEFAULT_INTERVAL_REQUEST_PATH_), web::http::methods::GET);
    this->price_data_target_ = target;

    // Set request to default 12 hour average for BTC -> USD
    setupRequest(DEFAULT_TICKER_FROM_, DEFAULT_TICKER_TO_, DEFAULT_TIME_IN_HOURS_);
	updateFullRequestPathInterval();
	updateInstantPriceRequestPath(utility::conversions::to_utf8string(DEFAULT_TICKER_FROM_));

}

Apollo::Bot::PriceData::~PriceData()
{
}

void Apollo::Bot::PriceData::saveSettings()
{
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
            param.value = ticker_from;
            return;
        }
    }
    this->price_data_target_.request_parameters.push_back(RequestParameter(U("fsym"), ticker_from));

	if (update_uri)
	{
		updateInstantPriceRequestPath(utility::conversions::to_utf8string(ticker_from));
		updateFullRequestPathInterval();
	}
		
}

void Apollo::Bot::PriceData::setTickerTo(utility::string_t ticker_to, bool update_uri = true)
{
    for (RequestParameter param : this->price_data_target_.request_parameters)
    {
        if (param.key == U("tsym"))
        {
            param.value = ticker_to;
            return;
        }
    }
    this->price_data_target_.request_parameters.push_back(RequestParameter(U("tsym"), ticker_to));

	// TODO: ADD EDITING TICKER_TO for INSTANT
	if (update_uri)
		updateFullRequestPathInterval();
		
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
    this->price_data_target_.request_parameters.push_back(RequestParameter(U("limit"), utility::conversions::to_string_t(std::to_string(time_in_hours * 60))));

    if (update_uri)
		updateFullRequestPathInterval();
}

void Apollo::Bot::PriceData::setupRequest(utility::string_t ticker_from, utility::string_t ticker_to, int time_in_hours)
{
    setTickerFrom(ticker_from, false);
    setTickerTo(ticker_to, false);
    setAveragePeriod(time_in_hours, false);
	updateFullRequestPathInterval();
	updateInstantPriceRequestPath(utility::conversions::to_utf8string(ticker_from));
}



void Apollo::Bot::PriceData::updateFullRequestPathInterval()
{
    uri_builder builder;
    builder.set_path(this->price_data_target_.request_path);
    for (auto param : this->price_data_target_.request_parameters)
        builder.append_query(param.key, param.value);

    this->full_request_path_interval_ = utility::conversions::to_utf8string(builder.to_string());
}

std::string Apollo::Bot::PriceData::requestIntervalPriceData()
{
    //sample https://min-api.cryptocompare.com/data/histominute?fsym=ETH&tsym=USD&limit=1440&toTs=1520380800

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
    req.set_request_uri(utility::conversions::to_string_t(this->full_request_path_interval_));

    client.request(req).then([](http_response res)
                             {
                                 return res.extract_utf8string();
                             }).then([&response](pplx::task<std::string> utf8str)
                                     {
                                         response = utf8str.get();
                                     }).wait();

    return response;
}

double Apollo::Bot::PriceData::getIntervalAverage()
{
    std::string json_as_string = requestIntervalPriceData();

    rapidjson::Document doc;
    doc.Parse(json_as_string);

    double total_avgprice_x_vol = 0.0;
    double total_vol = 0.0;

    for (auto& minuteData : doc["Data"].GetArray())
    {
        total_avgprice_x_vol += ((minuteData["high"].GetDouble() + minuteData["low"].GetDouble() + minuteData["close"].GetDouble()) / 3) * minuteData["volumeto"].GetDouble();
        total_vol += minuteData["volumeto"].GetDouble();
    }

    return (total_avgprice_x_vol / total_vol);
}

void Apollo::Bot::PriceData::updateInstantPriceRequestPath(std::string ticker)
{
    this->full_request_path_instant_ = "/data/price?fsym=" + ticker + "&tsyms=BTC";
}

std::string Apollo::Bot::PriceData::requestLastPrice()
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

    req.set_request_uri(utility::conversions::to_string_t(full_request_path_instant_));

    client.request(req).then([](http_response res)
                             {
                                 return res.extract_utf8string();
                             }).then([&response](pplx::task<std::string> utf8str)
                                     {
                                         response = utf8str.get();
                                     }).wait();

    return response;
}

double Apollo::Bot::PriceData::getLastPrice()
{
    std::string json_as_string = requestLastPrice();

    rapidjson::Document doc;
    doc.Parse(json_as_string);

	// BREAKS RIGHT HERE!!!!!!!!!! //
    return doc["BTC"].GetDouble();
}