//
// Created by Roderic Deichler on 2/20/18.
//

#if defined(_WIN32) && !defined(__cplusplus_winrt)
// Extra includes for Windows desktop.
#include <windows.h>
#include <Shellapi.h>
#endif

#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include "RedditOauth.h"
#include <iostream>
#include <sys/stat.h>
#include <string>

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

inline bool exist(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
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
static const utility::string_t s_reddit_code(U("0RduF6WUMg5N8HA_q96el1rjcGc"));
static const utility::string_t s_reddit_refresh_token(U("73830480229-vaJRUR9ihfhJYiRVYPQ-BcYPrHg"));


#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    RedditOauth reddit(s_reddit_client,s_reddit_response_type,s_reddit_state,s_reddit_redirect_uri,
                       s_reddit_duration,s_reddit_scope, s_reddit_secret, U("VeChain"));

    //ucout << reddit.buildRedditOauthURL() << std::endl;

    //if (!exist("../resources/reddit_token_data.txt")) {
    //reddit.setTokens(s_reddit_client, s_reddit_secret, s_reddit_code, s_reddit_redirect_uri);
    //   reddit.getTokens();
    //}

    reddit.readSubscriberCount(s_reddit_client, s_reddit_secret, s_reddit_code);

    ucout << std::endl;
    ucout << std::endl;

    reddit.readComments(s_reddit_client, s_reddit_secret, s_reddit_code);
    //else {
    reddit.refreshTokens(s_reddit_client, s_reddit_secret, s_reddit_refresh_token);
    //}

    reddit.readSubscriberCount(s_reddit_client, s_reddit_secret, s_reddit_code);

    reddit.refreshTokens(s_reddit_client, s_reddit_secret, s_reddit_refresh_token);

    //ucout << response_body;

   // ucout << "Done." << std::endl;
    return 0;
}