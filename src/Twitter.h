#ifndef TWITTER_H
#define TWITTER_H

#include "Bot.h"

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
            std::string oauth_access_token_secret;
        protected:
            virtual std::stringstream requestResponse(const std::string& target_url);
            virtual std::vector<Comment> parseJSON(const rapidjson::Document& document) = 0;
            virtual std::vector<Comment> cleanComments(std::vector<Comment>& comments) = 0;
        public:
        };
    }
}
#endif