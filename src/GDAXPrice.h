#ifndef APOLLOTRADINGBOT_GDAXPRICE_H
#define APOLLOTRADINGBOT_GDAXPRICE_H

#include <cpprest/details/basic_types.h>
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"

namespace Apollo {
    double getAskingPriceGdax(utility::string_t coin_id);
}

#endif //APOLLOTRADINGBOT_GDAXPRICE_H
