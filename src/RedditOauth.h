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

public:

    RedditOauth() {};
    RedditOauth(utility::string_t client, utility::string_t response_type,
                utility::string_t state, utility::string_t redirect_uri,
                utility::string_t duration, utility::string_t scope);
    utility::string_t buildRedditOauthURL();
};

RedditOauth::RedditOauth(utility::string_t client, utility::string_t response_type, utility::string_t state,
                         utility::string_t redirect_uri, utility::string_t duration, utility::string_t scope)
{
    s_reddit_client = client;
    s_reddit_response_type = response_type;
    s_reddit_state = state;
    s_reddit_redirect_uri = redirect_uri;
    s_reddit_duration = duration;
    s_reddit_scope = scope;
    s_reddit_auth_endpoint = U("https://www.reddit.com/api/v1/authorize?");
    s_reddit_token_endpoint = U("https://www.reddit.com/api/v1/access_token");
}

utility::string_t RedditOauth::buildRedditOauthURL() {
    //utility::string_t question(U("?"));
    utility::string_t amp(U("&"));
    utility::string_t idEquals(U("client_id="));
    utility::string_t respEquals(U("response_type="));
    utility::string_t redUri(U("redirect_uri="));
    utility::string_t duration(U("duration="));
    utility::string_t state(U("state="));
    utility::string_t scope(U("scope="));


    utility::string_t oauth_url = s_reddit_auth_endpoint + idEquals + s_reddit_client + amp + respEquals +
                                  s_reddit_response_type + amp + state + s_reddit_state + amp + redUri +
                                  s_reddit_redirect_uri + amp + duration + s_reddit_duration + amp +
                                  scope + s_reddit_scope;

    return oauth_url;
}

#endif //APOLLOTRADINGBOT_REDDITOAUTH_H
