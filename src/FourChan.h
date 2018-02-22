#ifndef FOURCHAN_H
#define FOURCHAN_H

#include "Bot.h"

namespace Apollo
{
    namespace Bot
    {
        class FourChan : public Bot
        {
        private:
            //fields
            const std::string RESOURCE_FILE_ = "../resources/fourchan.txt";
            const utility::string_t BASE_URL_ = U("https://a.4cdn.org");

            //methods
            virtual void saveSettings() override;
        protected:
            //fields
            unsigned long long int highest_post_seen_;
            unsigned long long int highest_timestamp_seen_;
            std::vector<std::string> search_patterns_;

            //methods
            virtual std::string requestResponse(const ScraperTarget& target) override;
            virtual std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            virtual std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
        public:
            FourChan();
            virtual ~FourChan();

            //methods
            virtual void addSearchQuery(const std::string& query, size_t number_of_results) override;
        };
    }// end of Bot namespace
}// end of Apollo namespace
#endif
