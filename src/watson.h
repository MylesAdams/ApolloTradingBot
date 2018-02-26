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


    private: // Helper functions.
        Watson();                                                                               // Default constructor.
        int rateTone(const utility::string_t &tone);                                            // Helper. Rates tones. -1, 0, or 1,
        void evaluator(web::json::value& json_sentiment, double& pos, double& neg);             // Helper. Gets max pos/neg tone.

    private: // Forbiden functions.
        Watson(Watson &copy) = delete;                       // Forbid copy constructor.
        Watson &operator=(Watson &copy) = delete;            // Forbid assignment.

    public: // Public functions.

        // Overloaded constructor ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This is intended to be the only method by which a watson object can be instantiated. We require that username and password be supplied
        // on instantiation so that the constructor can perform essential tasks of building the client and request and so checks can be performed
        // on those components prior to any calls being to the watson API.
        //
        //                @author: Andrew Laux
        //             Parameters: string_t user, pass - Username and password of watson service credentials.
        //          Return values: None - Constructor
        //         pre-conditions: Must have an active account with IBM cloud development that has a subscription to Watson Tone Analyzer.
        //                         The service credentials passed as arguments must correspond with said IBM account and watson subscription.
        //        post-conditions: Config will be set with valid credentials. URI builder will will pass a valid URI to http_request.
        //      Exceptions thrown: Bad URI throws std::ivalid_argument. Credentials not set fails assertion.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Watson(utility::string_t user, utility::string_t pass);

        // Public Method: toneToString //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function takes a comment of string_t type by const ref and sends it via the body of an http request using POST method. Implements
        // asychornous code to ensure that no  processing is being done before the reposonse from the server has fully arrived. Outputs the
        // reponse json in the form of a string.
        //
        //                @author: Andrew Laux
        //             Parameters: string_t tone_input - Contains the comment to be analyzed representd as a string.
        //          Return values: string_t - Response from watson in the form of a string.
        //         pre-conditions: The client which instantiated the Apollo::Watson onject must have passed valid credentials.
        //        post-conditions: None
        //      Exceptions thrown: Bad server status_code throws Apollo::Bot::BadStatusException.
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        utility::string_t toneToString(const utility::string_t &tone_input);


        // Public Method: toneToJson ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function takes a comment of string_t type by const ref and sends it via the body of an http request using POST method. Implements
        // asychornous code to ensure that no  processing is being done before the reposonse from the server has fully arrived. Outputs the
        // reponse json in the form of a web::json::value.
        //
        //                @author: Andrew Laux
        //             Parameters: string_t tone_input - Contains the comment to be analyzed representd as a string.
        //          Return values: web::json::value - Contains sentiment returned from watson.
        //         pre-conditions: The client which instantiated the Apollo::Watson onject must have passed valid credentials.
        //        post-conditions: None
        //      Exceptions thrown: Bad server status_code throws Apollo::Bot::BadStatusException.
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        web::json::value toneToJson(const utility::string_t &tone_input);

        // Public Method: toneToFile /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Function calls toneToJson method and then sends the resulting json to helper function called evaluator. Evaluator categorizes the
        // sentiment output of Watson in a way that can be graphed and outputs the result to file. The file must be specified by caller, if no
        // such file exists one will be created.
        //
        //                @author: Andrew Laux
        //             Parameters: string_t tone_input - Contains the comment to be analyzed representd as a string.
        //                         string_t file_name - Name of the file to  output to.
        //          Return values: None
        //         pre-conditions: That http request and client were built correctly.
        //        post-conditions: Output .json file is readable and graphable by GUI.
        //      Exceptions thrown: Throws exceptions generated by toneToString and toneToJson
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void toneToFile(utility::string_t &tone_input, const utility::string_t &file_name);       // Throws exception.
    };
}

#endif // !watson_h
