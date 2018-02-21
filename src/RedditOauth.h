//
// Created by Roderic Deichler on 2/20/18.
//

#ifndef APOLLOTRADINGBOT_REDDITOAUTH_H
#define APOLLOTRADINGBOT_REDDITOAUTH_H

#include <cpprest/details/basic_types.h>
#include <string>

class RedditOauth
{
private:

    utility::string_t s_reddit_client;
    utility::string_t s_reddit_response_type;
    utility::string_t s_reddit_state;
    utility::string_t s_reddit_redirect_uri;
    utility::string_t s_reddit_duration;
    utility::string_t s_reddit_scope;
    utility::string_t s_reddit_auth_endpoint;
    utility::string_t s_reddit_token_endpoint;
    utility::string_t s_reddit_secret;
    utility::string_t current_token;
    utility::string_t refresh_token;
    bool hasToken = false;

public:

    RedditOauth() {};

    RedditOauth(utility::string_t client, utility::string_t response_type,
                utility::string_t state, utility::string_t redirect_uri,
                utility::string_t duration, utility::string_t scope,
                utility::string_t secret);

    utility::string_t buildRedditOauthURL();
    void setTokens(utility::string_t client,utility::string_t secret,utility::string_t code,utility::string_t redirect_uri);
};


#endif //APOLLOTRADINGBOT_REDDITOAUTH_H
