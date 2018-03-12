//
// Created by Roderic Deichler on 2/20/18.
//
#include "Reddit.h"
#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

Apollo::Bot::Reddit::Reddit()
{
    s_reddit_subreddit = U("");

    std::string line;
    std::ifstream tokenFile("../resources/reddit_token_data.txt");
    if (tokenFile.is_open())
    {
        getline(tokenFile, line);
        UTC_time = stoi(line);
        tokenFile.close();
    }
    else UTC_time = 0;
}

Apollo::Bot::Reddit::Reddit(utility::string_t subreddit)
{
    s_reddit_subreddit = subreddit;

    std::string line;
    std::ifstream tokenFile("../resources/reddit_token_data.txt");
    if (tokenFile.is_open())
    {
        getline(tokenFile, line);
        UTC_time = stoi(line);
        tokenFile.close();
    }
    else UTC_time = 0;
}

utility::string_t Apollo::Bot::Reddit::buildRedditOauthURL()
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

void Apollo::Bot::Reddit::setTokens()
{
    credentials cred(s_reddit_client, s_reddit_secret);
    http_client_config config;

    config.set_credentials(cred);

    http_client redditClient(U("https://www.reddit.com/api/"), config);

    http_request reqRed(methods::POST);
    uri_builder builder(U("v1/access_token"));
    builder.append_query(U("grant_type"), U("authorization_code"));
    builder.append_query(U("code"), s_reddit_current_token);
    builder.append_query(U("redirect_uri"), s_reddit_redirect_uri);

    reqRed.set_request_uri(builder.to_string());

    reqRed.headers().add(U("user"), s_reddit_client);
    reqRed.headers().add(U("password"), s_reddit_secret);

    json::value response_body;

    pplx::task<http_response> reddit_call = redditClient.request(reqRed);
    reddit_call

            .then([](http_response response) {
//                std::cout << response.status_code() << std::endl;

                return response.extract_json();

            }).then([&response_body](pplx::task<json::value > json_task){

                response_body = json_task.get();

            }).wait();

    s_reddit_current_token = utility::conversions::to_string_t(response_body[U("access_token")].as_string());

    std::ofstream tokenFile;
    tokenFile.open("../resources/reddit_token_data.txt");
    tokenFile << utility::conversions::to_utf8string(s_reddit_current_token) << "\n" << utility::conversions::to_utf8string(s_reddit_refresh_token);
    tokenFile.close();

}

void Apollo::Bot::Reddit::getTokens()
{
    std::string line;
    std::ifstream tokenFile("../resources/reddit_token_data.txt");
    if (tokenFile.is_open())
    {
        int count = 0;
        while (getline(tokenFile,line))
        {
            if (count == 0)
                s_reddit_current_token = utility::conversions::to_string_t(line);
            else
            {
                //s_reddit_refresh_token = line;
                count++;
            }
        }
        tokenFile.close();
    }

    else
        std::cout << "Unable to open file";
}

void Apollo::Bot::Reddit::refreshTokens()
{
    int_fast64_t UTC_temp = utility::datetime::utc_timestamp();
    if (UTC_temp - 3590 < UTC_time)
    {
//        ucout << "no need to refresh token yet" << std::endl;
        return;
    }


    credentials cred(s_reddit_client, s_reddit_secret);
    http_client_config config;

    config.set_credentials(cred);

    http_client redditClient(U("https://www.reddit.com/api/"), config);

    http_request reqRed(methods::POST);
    uri_builder builder(U("v1/access_token"));
    builder.append_query(U("grant_type"), U("refresh_token"));
    builder.append_query(U("refresh_token"), s_reddit_refresh_token);

    reqRed.set_request_uri(builder.to_string());

    reqRed.headers().add(U("user"), s_reddit_client);
    reqRed.headers().add(U("password"), s_reddit_secret);


    json::value response_body;

    pplx::task<http_response> reddit_call = redditClient.request(reqRed);
    reddit_call

            .then([](http_response response) {
//                std::cout << response.status_code() << std::endl;

                return response.extract_json();

            })
            .then([&response_body](pplx::task<json::value > json_task){

                response_body = json_task.get();

            })
            .wait();

    s_reddit_current_token = response_body[U("access_token")].as_string();
    UTC_time = UTC_temp;
    std::ofstream tokenFile;
    tokenFile.open("../resources/reddit_token_data.txt");
    tokenFile << UTC_time << "\n";
    tokenFile.close();

}

void Apollo::Bot::Reddit::readComments()

{
    refreshTokens();
    http_client redditClient(U("https://oauth.reddit.com/r/") + s_reddit_subreddit + U("/comments.json"));

    http_request reqRed(methods::GET);

    reqRed.headers().add(U("user"), s_reddit_client);
    reqRed.headers().add(U("password"), s_reddit_secret);
    reqRed.headers().add(U("Authorization"), U("bearer ") + s_reddit_current_token);

    comments.clear();

//    json::value response_body;

    std::string response;

    pplx::task<http_response> reddit_call = redditClient.request(reqRed);
    reddit_call

            .then([](http_response response) {
//                std::cout << response.status_code() << std::endl;

//                return response.extract_json();
                return response.extract_vector();

            }).then([&response](pplx::task<std::vector<unsigned char>> vector_task){
                auto v = vector_task.get();
                response.assign(v.begin(), v.end());

            }).wait();

    rapidjson::Document response_body;
    response_body.Parse(response.c_str());

    size_t tempLastRead = response_body["data"]["children"][0]["data"]["created"].GetDouble();

    for (int i = 0; i < response_body["data"]["children"].Size(); i++)
        comments.push_back(utility::conversions::to_string_t(response_body["data"]["children"][i]["data"]["body"].GetString()));

    last_comment_read = tempLastRead;

}

void Apollo::Bot::Reddit::readSubscriberCount()
{
    refreshTokens();

    http_client redditClient(U("https://oauth.reddit.com/r/") + s_reddit_subreddit + U("/about.json"));

    http_request reqRed(methods::GET);

    reqRed.headers().add(U("user"), s_reddit_client);
    reqRed.headers().add(U("password"), s_reddit_secret);
    reqRed.headers().add(U("Authorization"), U("bearer ") + s_reddit_current_token);

    json::value response_body;

    pplx::task<http_response> reddit_call = redditClient.request(reqRed);
    reddit_call

            .then([](http_response response) {
//                std::cout << response.status_code() << std::endl;

                return response.extract_json();

            })
            .then([&response_body](pplx::task<json::value > json_task){

                response_body = json_task.get();

            })
            .wait();

    subscriber_count = response_body[U("data")][U("subscribers")].as_integer();
    ucout << "subcribers: " << subscriber_count << std::endl;
}

void Apollo::Bot::Reddit::setSubreddit(utility::string_t subreddit)
{
    s_reddit_subreddit = subreddit;
}

std::vector<utility::string_t> Apollo::Bot::Reddit::getComments()
{
    return comments;
}

void Apollo::Bot::Reddit::saveSettings() {}

std::string Apollo::Bot::Reddit::requestResponse(const ScraperTarget& target) {return "";}

std::vector<Apollo::Comment> Apollo::Bot::Reddit::parseJSON(const rapidjson::Document& document)
{
    std::vector<Apollo::Comment> ret;
    return ret;
}
std::vector<Apollo::Comment> Apollo::Bot::Reddit::cleanComments(std::vector<Apollo::Comment>& comments)
{
    std::vector<Apollo::Comment> ret;
    return ret;
}

void Apollo::Bot::Reddit::setSearchQuery(const std::string& query){}
