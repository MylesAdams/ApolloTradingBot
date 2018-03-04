#include "GDAXPrice.h"
#include <cpprest/details/basic_types.h>
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include "rapidjson/rapidjson.h"


double Apollo::getAskingPriceGdax(utility::string_t coin_id) {
    double price;
    utility::string_t request_path = U("/products/") + coin_id + U("-USD/ticker");

    web::http::http_request req(web::http::methods::GET);
    req.set_request_uri(request_path);
    req.headers().add(U("CB-ACCESS-KEY"), U("b2d3e3ca35f4c97cc416ba79b89e62b8"));
    req.headers().set_content_type(U("application/json"));

    web::http::client::http_client myclient(U("https://api.gdax.com"));
    pplx::task<web::http::http_response> call = myclient.request(req);

    call.then([](web::http::http_response response) {
//        std::cout << response.status_code() << std::endl;
        return response.extract_json();
    }).then([&price](web::json::value json) {
        auto auto_val = json[U("price")];
        price = std::stod(utility::conversions::to_utf8string(auto_val.as_string()));
    }).wait();
    return price;
}

