/*
* Copyright (C) Microsoft. All rights reserved.
* Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
*
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*
* Oauth2Client.cpp : Defines the entry point for the console application
*
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
****/
/*
INSTRUCTIONS
This sample performs authorization code grant flow on various OAuth 2.0
services and then requests basic user information.
This sample is for Windows Desktop, OS X and Linux.
Execute with administrator privileges.
Set the app key & secret strings below (i.e. s_reddit_key, s_reddit_secret, etc.)
To get key & secret, register an app in the corresponding service.
Set following entry in the hosts file:
127.0.0.1    testhost.local
*/
//#include "stdafx.h"

#if defined(_WIN32) && !defined(__cplusplus_winrt)
// Extra includes for Windows desktop.
#include <windows.h>
#include <Shellapi.h>
#endif

#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include "RedditOauth.h"
#include <iostream>


using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

std::string setRandString()
{
    char randString[10];
    for (int ndx = 0; ndx < sizeof(randString)-1; ndx++)
        randString[ndx] = 'A' + rand()%26;
    return randString;
}


//
// Set key & secret pair to enable session for that service.
//
static const utility::string_t s_reddit_secret(U("nH8HZW7W8X-meUt15cZA2kFfVD8"));

static const utility::string_t s_reddit_client(U("UH7RWCuYA_qBIA"));
static const utility::string_t s_reddit_response_type(U("code"));
static const utility::string_t s_reddit_state(U(setRandString()));
static const utility::string_t s_reddit_redirect_uri(U("https://mylesadams.github.io/ApolloTradingBot/"));
static const utility::string_t s_reddit_duration(U("permanent"));
static const utility::string_t s_reddit_scope(U("read"));
static const utility::string_t s_reddit_code(U("ugWP443x3sci9NzJKZ-3o9q1NGo"));

//
// Specialized class for Dropbox OAuth 2.0 session.
//
//class reddit_session_sample : public OauthSession
//{
//public:
//    reddit_session_sample() :
//        OauthSession(U("Reddit"),
//            s_reddit_key,
//            s_reddit_secret,
//            U("https://www.reddit.com/api/v1/authorize?"),
//            U("https://www.reddit.com/api/v1/access_token"),
//            U("https://mylesadams.github.io/ApolloTradingBot/"))
////            U("read"),
////            U("Apollo_Scaper"))
//    {
//        // Dropbox uses "default" OAuth 2.0 settings.
//    }
//    void run_internal() override
//    {
//        http_client api(U("https://reddit.com/api/v1/authorize?"), m_http_config);
//        ucout << "Requesting account information:" << std::endl;
//        ucout << "Information: " << api.request(methods::GET, U("account/info")).get().extract_json().get() << std::endl;
//    }
//};


#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    /*credentials cred(s_reddit_client, s_reddit_secret);
    http_client_config config;

    config.set_credentials(cred);

    http_client redditClient(U("https://www.reddit.com/api/"), config);

    http_request reqRed(methods::POST);
    uri_builder builder("v1/access_token");
    builder.append_query(U("grant_type"), U("authorization_code"));
    builder.append_query(U("code"), U("4wAFGtsuR7M7H79A1FbGJxJcl08"));
    builder.append_query(U("redirect_uri"), U("https://mylesadams.github.io/ApolloTradingBot/"));

    reqRed.set_request_uri(builder.to_string());

    reqRed.headers().add(U("user"), s_reddit_client);
    reqRed.headers().add(U("password"), s_reddit_secret);

    ucout << "Running OAuth 2.0 client sample..." << std::endl;

    string_t response_body;

    pplx::task<http_response> reddit_call = redditClient.request(reqRed);
    reddit_call

            .then([](http_response response) {
        std::cout << response.status_code() << std::endl;

                return response.extract_string();

    })
            .then([&response_body](pplx::task<string_t> string_task){

                response_body = string_task.get();

            })

            .wait();*/

    RedditOauth reddit(s_reddit_client,s_reddit_response_type,s_reddit_state,s_reddit_redirect_uri,
                       s_reddit_duration,s_reddit_scope, s_reddit_secret);

    reddit.setTokens(s_reddit_client, s_reddit_secret, s_reddit_code, s_reddit_redirect_uri);
    ucout << reddit.buildRedditOauthURL() << std::endl;

    //ucout << response_body;

   // ucout << "Done." << std::endl;
    return 0;
}