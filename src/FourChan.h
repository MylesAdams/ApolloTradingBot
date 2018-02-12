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
        protected:
            std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
        public:
            FourChan(const std::vector<std::string>& complete_urls);
            FourChan(const std::vector<std::string>& complete_urls, const std::vector<std::string>& incomplete_urls);
        };
    }// end of Bot namespace
}// end of Apollo namespace
#endif FOURCHAN_H