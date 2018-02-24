// File: watson.h
// Written by ANDREW LAUX

#ifndef watson_h
#define watson_h

#include "BadStatusException.h"

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <assert.h>

namespace Apollo
{
// Class: Watson
    class Watson
    {
    private: // Data members.
        web::http::client::http_client_config config;           // Passed to client constructor. Holds username and pass.
        web::http::http_request req;                            // Http request object. Has method and uri.

    public: // Public functions.
        Watson(utility::string_t user, utility::string_t pass);                                         // Explicit contructor.
        utility::string_t toneToString(const utility::string_t &tone_input);                            // Throws exception.
        web::json::value toneToJson(const utility::string_t &tone_input);                               // Throws exception.
        void toneToFile(const utility::string_t &tone_input, const utility::string_t &file_name);       // Throws exception.

    private: // Helper functions.
        Watson();                                                                               // Default constructor.
        int rateTone(const utility::string_t &tone);                                            // Helper. Rates tones. -1, 0, or 1,
        void evaluator(web::json::value& json_sentiment, double& pos, double& neg);       // Helper. Gets max pos/neg tone.

    private: // Forbiden functions.
        Watson(Watson &copy) = delete;                       // Forbid copy constructor.
        Watson &operator=(Watson &copy) = delete;            // Forbid assignment.
    };
}

#endif // !watson_h
