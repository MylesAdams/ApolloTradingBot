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
            std::string oauth_access_token_secret_;
            const std::string resource_file_ = "../resources/twitter.json";
            unsigned long long highest_timestamp_seen_;
            std::vector<std::string> request_paths_;
            std::string percentEncode(const utility::string_t& s);

        protected:
            virtual void saveSettings() override;
            virtual std::stringstream requestResponse(const ScraperTarget& target) override;
            virtual std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            virtual std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
            
        public:
            Twitter();
            virtual ~Twitter();
        };
    }
}
#endif