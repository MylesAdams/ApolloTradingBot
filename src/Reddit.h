//
// Created by Roderic Deichler on 2/16/18.
//

#ifndef APOLLOTRADINGBOT_REDDIT_H
#define APOLLOTRADINGBOT_REDDIT_H

#include "Bot.h"
#include <string>
#include <vector>

namespace Apollo
{
    namespace Bot
    {
        class Reddit : public Bot
        {
        private:
            std::string subscriberCount;

        protected:
            //using bot superclass COMPLETE URLS

            unsigned long int last_comment_read_;

            std::vector<std::string> COMPLETE_URLS_REDDIT_COUNT;
            std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
            std::vector<std::string> parseJSONSubscriberCount(const rapidjson::Document& document);

        public:
            Reddit();
            explicit Reddit(std::string subreddit);
            virtual ~Reddit();
        };

    }

}


#endif
