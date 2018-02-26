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
#include "Bot.h"

namespace Apollo
{
    namespace Bot
    {
        class Reddit : public Bot
        {
        protected:
            virtual void saveSettings() override;
            virtual std::string requestResponse(const ScraperTarget& target) override;
            virtual std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            virtual std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
            void setSearchQuery(const std::string& query) override;

        private:
            const utility::string_t s_reddit_secret = (U("nH8HZW7W8X-meUt15cZA2kFfVD8"));
            const utility::string_t s_reddit_client = (U("UH7RWCuYA_qBIA"));
            const utility::string_t s_reddit_response_type = (U("code"));
            const utility::string_t s_reddit_state = (utility::conversions::to_string_t(setRandString()));
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
            size_t subscriber_count;
            bool hasToken = false;
            std::vector<std::string> comments;

            utility::string_t buildRedditOauthURL();

            utility::string_t getRefreshTokenFromFile();

            utility::string_t setRefreshTokenFile();

            void setTokens();

            void getTokens();

            void refreshTokens();

            void refreshTokens2();

            //helper function for setting the state
            std::string setRandString()
            {
                char randString[10];
                for (int ndx = 0; ndx < sizeof(randString) - 1; ndx++)
                    randString[ndx] = 'A' + rand() % 26;
                return randString;
            }



        public:

            /*
            @author: Roderic Deichler
            Parameters: None - default constructor
            Return values: None - default constructor
            pre-conditions: The resource file "reddit_token_data.txt" in the resources folder should exist.
            post-conditions: The Reddit object is constructed and initialized. The subreddit to search must be set before doing any searches.
            */
            Reddit();

            /*
            @author: Roderic Deichler
            Parameters: subreddit: A string_t that represents the subreddit on reddit.com that will be searched.
            Return values: None - constructor
            pre-conditions: The resource file "reddit_token_data.txt" in the resources folder should exist.
            post-conditions: The Reddit object is constructed and initialized. Searches are ready to be executed.
            */
            Reddit(utility::string_t subreddit);

            /*
            @author: Roderic Deichler
            Parameters: subreddit: A string_t that represents the subreddit on reddit.com that will be searched.
            Return values: None
            pre-conditions: Reddit object must be initialized.
            post-conditions: The subreddit to be searched is set.
            */
            void setSubreddit(utility::string_t subreddit);

            /*
            @author: Roderic Deichler
            Parameters: none
            Return values: None - constructor
            pre-conditions: Subreddit to read from must be set.
            post-conditions: Comments since the last search will be grabbed and stored in the comments vector.
            */
            void readComments();

            /*
            @author: Roderic Deichler
            Parameters: none
            Return values: None - constructor
            pre-conditions: Subreddit to read from must be set.
            post-conditions: Subcriber count of the subreddit be grabbed and stored in the subscriber_count value.
            */
            void readSubscriberCount();

            /*
            @author: Myles Adams
            Parameters: none
            Return values: std::vector<utility::string_t> : Vector of comments
            pre-conditions: none
            post-conditions: Comments in the private comments vector will be returned.
            */
            std::vector<std::string> getComments();





        };
    }
}

#endif //APOLLOTRADINGBOT_REDDITOAUTH_H
