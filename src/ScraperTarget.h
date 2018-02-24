#ifndef SCRAPERTARGET_H
#define SCRAPERTARGET_H

#include "RequestParameter.h"
#include "cpprest/details/basic_types.h"
#include <vector>
#include <cpprest/http_client.h>

namespace Apollo
{
    namespace Bot
    {
        struct ScraperTarget //TODO -- make this into a class with a .h and .cpp file. Literally make it exactly the same but as a class.
        {
            utility::string_t resource_url;
            utility::string_t request_path;
            web::http::method request_method;
            std::vector<RequestParameter> request_parameters;
            ScraperTarget() : resource_url(U("")), request_path(U("")), request_method(web::http::methods::GET) {}
            ScraperTarget(const utility::string_t& resource_url, const utility::string_t& request_path, const web::http::method& request_method) :
                resource_url(resource_url),
                request_path(request_path),
                request_method(request_method)
            {}
        };
    }
}
#endif