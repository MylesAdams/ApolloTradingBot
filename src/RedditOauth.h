//
// Created by Roderic Deichler on 2/20/18.
//

#ifndef APOLLOTRADINGBOT_REDDITOAUTH_H
#define APOLLOTRADINGBOT_REDDITOAUTH_H

#include <cpprest/details/basic_types.h>
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include <string>
#include <sys/stat.h>
#include <vector>

class RedditOauth
{
private:
    const utility::string_t s_reddit_secret = (U("nH8HZW7W8X-meUt15cZA2kFfVD8"));
    const utility::string_t s_reddit_client = (U("UH7RWCuYA_qBIA"));
    const utility::string_t s_reddit_response_type = (U("code"));
    const utility::string_t s_reddit_state = (U(setRandString()));
    const utility::string_t s_reddit_redirect_uri = (U("https://mylesadams.github.io/ApolloTradingBot/"));
    const utility::string_t s_reddit_duration = (U("permanent"));
    const utility::string_t s_reddit_scope = (U("read"));
    const utility::string_t s_reddit_refresh_token = (U("73830480229-4ro_odcRSTrFag6YiDk7n0CBt8s"));
    const utility::string_t s_reddit_auth_endpoint = (U("https://www.reddit.com/api/v1/authorize?"));
    const utility::string_t s_reddit_token_endpoint = (U("https://www.reddit.com/api/v1/access_token"));

    utility::string_t s_reddit_subreddit;
    utility::string_t s_reddit_current_token;

    int_fast64_t UTC_time;
    size_t last_comment_read = 0;
    std::vector<utility::string_t> comments;
    size_t subscriber_count;
    bool hasToken = false;

public:

    RedditOauth() {};

    RedditOauth(utility::string_t subreddit);

    utility::string_t buildRedditOauthURL();
    utility::string_t getRefreshTokenFromFile();
    utility::string_t setRefreshTokenFile();

    void readComments();
    void readSubscriberCount();

    void setTokens();
    void getTokens();
    void refreshTokens();
    void refreshTokens2();

    //helper function for setting the state
    std::string setRandString()
    {
        char randString[10];
        for (int ndx = 0; ndx < sizeof(randString)-1; ndx++)
            randString[ndx] = 'A' + rand()%26;
        return randString;
    }
};


#endif //APOLLOTRADINGBOT_REDDITOAUTH_H
