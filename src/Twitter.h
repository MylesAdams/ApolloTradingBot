#ifndef TWITTER_H
#define TWITTER_H

#include "Bot.h"
#include <algorithm>

namespace Apollo
{
    namespace Bot
    {
        class Twitter : public Bot
        {
        private:
            std::string consumer_key_;
            std::string consumer_secret_;
            std::string oauth_access_token_key_;
            std::string oauth_access_token_secret_;
            const std::string RESOURCE_FILE_ = "../resources/twitter.json";

            //methods
            std::string percentEncode(const utility::string_t& s);
            const utility::string_t BASE_URL_ = U("https://api.twitter.com");
            const utility::string_t MAX_SEARCH_COUNT_ = U("300");

        protected:
            virtual void saveSettings() override;
            virtual std::string requestResponse(const ScraperTarget& target) override;
            virtual std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            virtual std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
            
        public:
            //ctor
            /*
            @author: Gavin Frazar
            Parameters: None - default constructor
            Return values: None - default constructor
            pre-conditions: The resource file (ApolloTradingBot/resources/twitter.json) must exist and be properly configured. Properly configured means:
                            -Must contain five key/value pairs in json format. If all five key/values exist, every value except the timestamp value must contain some string.
                            Empty value strings are ALWAYS invalid for the consumer_key, consumer_secret, and oauth access key/secret values.
            post-conditions: The Twitter object is constructed and initialized with four keys and a timestamp. A properly formatted twitter.json file will be generated if there was no such file before.
            Exceptions thrown: The constructor will throw if any pre-condition is not met. The following exceptions may be thrown:
                                -BadResourceException
            */
            Twitter();

            /*
            @author: Gavin Frazar
            Parameters: none - destructor
            Return values: none - destructor
            pre-conditions: none
            post-conditions: settings will be saved to the ApolloTradingBot/resources/twitter.json file in json format.
            Exceptions: none. Never throws.
            */
            virtual ~Twitter();

            //public methods
            /*
            @author: Gavin Frazar
            Parameters: query - string containing the query a user would like to search all of Twitter for.
            Return values: void
            pre-conditions: The query must be no more than 500 characters long.
            post-conditions: Twitter's internal ScraperTarget must be set and contain a correct url, request path, and 5 RequestParameter objects in its vector, as follows:
                                - "count" : "[0-9]+"
                                - "tweet_mode" : "extended"
                                - "q" : "query" (the query must be no longer than 500 characters)
                                - "result_type" : "recent"
            Exceptions: This method will throw if the pre-condition is not met. The following exception(s) can be thrown:
                        -BadTargetException
            */
            void setSearchQuery(const std::string& query) override;
        };
    }
}
#endif