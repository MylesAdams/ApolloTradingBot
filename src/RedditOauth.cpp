//
// Created by Roderic Deichler on 2/20/18.
//
#include "RedditOauth.h"
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

RedditOauth::RedditOauth(utility::string_t client, utility::string_t response_type, utility::string_t state,
                         utility::string_t redirect_uri, utility::string_t duration, utility::string_t scope,
                         utility::string_t secret)
{
    s_reddit_client = client;
    s_reddit_response_type = response_type;
    s_reddit_state = state;
    s_reddit_redirect_uri = redirect_uri;
    s_reddit_duration = duration;
    s_reddit_scope = scope;
    s_reddit_auth_endpoint = U("https://www.reddit.com/api/v1/authorize?");
    s_reddit_token_endpoint = U("https://www.reddit.com/api/v1/access_token");
    s_reddit_secret = secret;
}

utility::string_t RedditOauth::buildRedditOauthURL()
{
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

void RedditOauth::setTokens(utility::string_t client, utility::string_t secret, utility::string_t code,
                           utility::string_t redirect_uri)
{
    credentials cred(client, secret);
    http_client_config config;

    config.set_credentials(cred);

    http_client redditClient(U("https://www.reddit.com/api/"), config);

    http_request reqRed(methods::POST);
    uri_builder builder("v1/access_token");
    builder.append_query(U("grant_type"), U("authorization_code"));
    builder.append_query(U("code"), code);
    builder.append_query(U("redirect_uri"), redirect_uri);

    reqRed.set_request_uri(builder.to_string());

    reqRed.headers().add(U("user"), client);
    reqRed.headers().add(U("password"), secret);

    json::value response_body;

    pplx::task<http_response> reddit_call = redditClient.request(reqRed);
    reddit_call

            .then([](http_response response) {
                std::cout << response.status_code() << std::endl;

                return response.extract_json();

            })
            .then([&response_body](pplx::task<json::value > json_task){

                response_body = json_task.get();

            })
            .wait();

    if (response_body["access_token"].is_string() && response_body["refresh_token"].is_string())
    {
        current_token = response_body["access_token"].as_string();
        refresh_token = response_body["refresh_token"].as_string();
    }

    ucout << response_body.serialize() << std::endl;
    ucout << current_token << std::endl;
    ucout << refresh_token << std::endl;
}
