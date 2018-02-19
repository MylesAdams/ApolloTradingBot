#ifndef TWITTER_H
#define TWITTER_H

#include "Bot.h"
#include <cpprest\http_client.h>
#include <cpprest\filestream.h>
#include <cpprest\json.h>  
#include <openssl\hmac.h>

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
            const std::string resource_file_ = "../res/twitter.json";
            unsigned long long highest_timestamp_seen_;
            std::vector<std::string> uris_;

        protected:
            virtual void saveSettings() override;
            virtual std::stringstream requestResponse(const std::string& target_url) override;
            virtual std::vector<Comment> parseJSON(const rapidjson::Document& document) = 0;
            virtual std::vector<Comment> cleanComments(std::vector<Comment>& comments) = 0;
        public:
            Twitter();
            virtual ~Twitter();
        };
    }
}
#endif