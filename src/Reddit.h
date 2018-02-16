//
// Created by Roderic Deichler on 2/16/18.
//

#ifndef APOLLOTRADINGBOT_REDDIT_H
#define APOLLOTRADINGBOT_REDDIT_H

#include "Bot.h"
#include <string>


namespace Apollo
{
    namespace Bot
    {
        class Reddit : public Bot
        {
        private:
            std::string subscriberCount;
            void

        protected:
            //using bot superclass COMPLETE URLS
            std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;

        public:

            Reddit();
            Reddit(std::string subreddit);
            virtual ~Reddit();
        };

    }

}


#endif
