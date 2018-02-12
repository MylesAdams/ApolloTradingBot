#ifndef FOURCHAN_H
#define FOURCHAN_H

#include <fstream>

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
            //init ctor
            FourChan();

            //fields
            unsigned long long int highest_post_seen_;

            //methods
            std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
        public:
            FourChan(const std::vector<std::string>& complete_urls);
            FourChan(const std::vector<std::string>& complete_urls, const std::vector<std::string>& incomplete_urls);
            virtual ~FourChan();
        };
    }// end of Bot namespace
}// end of Apollo namespace
#endif FOURCHAN_H