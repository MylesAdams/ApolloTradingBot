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
            std::vector<std::string> request_paths_;
            std::string percentEncode(const utility::string_t& s);
            const utility::string_t BASE_URL_ = U("https://api.twitter.com");

        protected:
            virtual void saveSettings() override;
            virtual std::string requestResponse(const ScraperTarget& target) override;
            virtual std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            virtual std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
            
        public:
            Twitter();
            virtual ~Twitter();

            //methods
            void addSearchQuery(const std::string& query, size_t number_of_results) override;
        };
    }
}
#endif