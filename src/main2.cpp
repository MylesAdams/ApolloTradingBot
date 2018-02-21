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
#include "OauthListener.h"
#include "OauthSession.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

//
// Set key & secret pair to enable session for that service.
//
static const utility::string_t s_reddit_key(U("UH7RWCuYA_qBIA"));
static const utility::string_t s_reddit_secret(U("nH8HZW7W8X-meUt15cZA2kFfVD8"));

static const utility::string_t s_linkedin_key(U(""));
static const utility::string_t s_linkedin_secret(U(""));

static const utility::string_t s_live_key(U(""));
static const utility::string_t s_live_secret(U(""));

//
// Specialized class for Dropbox OAuth 2.0 session.
//
class reddit_session_sample : public OauthSession
{
public:
    reddit_session_sample() :
        OauthSession(U("Reddit"),
            s_reddit_key,
            s_reddit_secret,
            U("https://www.reddit.com/api/v1/authorize?"),
            U("https://www.reddit.com/api/v1/access_token"),
            U("https://mylesadams.github.io/ApolloTradingBot/"))
//            U("read"),
//            U("Apollo_Scaper"))
    {
        // Dropbox uses "default" OAuth 2.0 settings.
    }
//https://reddit.com/api/v1/authorize?client_id=UH7RWCuYA_qBIA&response_type=code&state=Stdlkmsg&redirect_uri=https://mylesadams.github.io/ApolloTradingBot/&duration=temporary&scope=read,mysubreddits
protected:
    void run_internal() override
    {
        http_client api(U("https://reddit.com/api/v1/authorize?"), m_http_config);
        ucout << "Requesting account information:" << std::endl;
        ucout << "Information: " << api.request(methods::GET, U("account/info")).get().extract_json().get() << std::endl;
    }
};


#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    ucout << "Running OAuth 2.0 client sample..." << std::endl;


    reddit_session_sample  reddit;

    reddit.run();


    ucout << "Done." << std::endl;
    return 0;
}