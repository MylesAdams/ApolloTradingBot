#ifndef FOURCHAN_H
#define FOURCHAN_H

#include <fstream>
#include <regex>

#include "Bot.h"

namespace Apollo
{
    namespace Bot
    {
        class FourChan : public Bot
        {
        private:
            const std::string resource_file = "../resources/fourchan.txt";
            void saveSettings();
        protected:
            //fields
            unsigned long long int highest_post_seen_;
            unsigned long long int highest_timestamp_seen_;

            //methods
            std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
        public:
            FourChan();
            virtual ~FourChan();
        };
    }// end of Bot namespace
}// end of Apollo namespace
#endif FOURCHAN_H