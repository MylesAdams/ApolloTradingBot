//
// Created by Roderic Deichler on 2/20/18.
//
#include "RedditOauth.h"
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include <fstream>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

RedditOauth::RedditOauth(utility::string_t client, utility::string_t response_type, utility::string_t state,
                         utility::string_t redirect_uri, utility::string_t duration, utility::string_t scope,
                         utility::string_t secret, utility::string_t subreddit)
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
    s_subreddit = subreddit;
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

   // if (response_body["access_token"].is_string() && response_body["refresh_token"].is_string())
   // {
        current_token = response_body["access_token"].as_string();
        refresh_token = response_body["refresh_token"].as_string();

        std::ofstream tokenFile;
        tokenFile.open("../resources/reddit_token_data.txt");
        tokenFile << current_token << "\n" << refresh_token;
        tokenFile.close();
   // }

    ucout << response_body.serialize() << std::endl;
    //ucout << current_token << std::endl;
   // ucout << refresh_token << std::endl;
}

void RedditOauth::getTokens()
{
    utility::string_t line;
    std::ifstream tokenFile("../resources/reddit_token_data.txt");
    if (tokenFile.is_open())
    {
        int count = 0;
        while (getline(tokenFile,line))
        {
            if (count == 0)
                current_token = line;
            else
                refresh_token = line;
            count++;
        }
        tokenFile.close();
    }

    else
        std::cout << "Unable to open file";
}

void RedditOauth::refreshTokens(utility::string_t client, utility::string_t secret, utility::string_t refresh_token) {

    credentials cred(client, secret);
    http_client_config config;

    config.set_credentials(cred);

    http_client redditClient(U("https://www.reddit.com/api/"), config);

    http_request reqRed(methods::POST);
    uri_builder builder("v1/access_token");
    builder.append_query(U("grant_type"), U("refresh_token"));
    builder.append_query(U("refresh_token"), refresh_token);

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

    // if (response_body["access_token"].is_string() && response_body["refresh_token"].is_string())
    // {
    current_token = response_body["access_token"].as_string();
    refresh_token = response_body["refresh_token"].as_string();

    /*std::ofstream tokenFile;
    tokenFile.open("../resources/reddit_token_data.txt");
    tokenFile << current_token << "\n" << refresh_token;
    tokenFile.close();
    // }*/

    ucout << response_body.serialize() << std::endl;

}

void RedditOauth::readComments(utility::string_t client,
                               utility::string_t secret, utility::string_t access_token)

{
    http_client redditClient(U("https://oauth.reddit.com/r/") + s_subreddit + U("/comments.json"));

    http_request reqRed(methods::GET);

    reqRed.headers().add(U("user"), client);
    reqRed.headers().add(U("password"), secret);
    reqRed.headers().add(U("Authorization"), U("bearer ") + access_token);

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

    size_t tempLastRead = response_body["data"]["children"][0]["data"]["created"].as_integer();

    for (int i = 0; i < response_body["data"]["children"].size(); i++)
    {
        //if  (response_body["data"]["children"][i]["data"]["created"].as_integer() > last_comment_read)
        //{
            comments.push_back(response_body["data"]["children"][i]["data"]["body"].as_string());
        //}
    }

    last_comment_read = tempLastRead;

    for (int i = 0; i < comments.size(); i++)
        ucout << comments[i] << std::endl;
    ucout << "last comment read id: " << last_comment_read << std::endl;
    ucout << response_body.serialize();
}

void RedditOauth::readSubscriberCount(utility::string_t client,
                                      utility::string_t secret, utility::string_t access_token)
{

    http_client redditClient(U("https://oauth.reddit.com/r/") + s_subreddit + U("/about.json"));

    http_request reqRed(methods::GET);

    reqRed.headers().add(U("user"), client);
    reqRed.headers().add(U("password"), secret);
    reqRed.headers().add(U("Authorization"), U("bearer ") + access_token);

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

    subscriber_count = response_body["data"]["subscribers"].as_integer();
    //ucout << response_body.serialize() << std::endl;
    ucout << "subcribers: " << subscriber_count << std::endl;
}

