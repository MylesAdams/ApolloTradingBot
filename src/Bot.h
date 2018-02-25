#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "Comment.h"
#include "RequestParameter.h"
#include "ScraperTarget.h"
#include "BadResourceException.h"
#include "BadTargetException.h"
#include "BadStatusException.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>  
#include <openssl/hmac.h>

namespace Apollo {
    namespace Bot { // Put all classes that extend Bot in the Bot namespace
        class Bot
        {
        private:

        protected:
            //ctors
            Bot();
            virtual ~Bot();

            //fields
            Apollo::Bot::ScraperTarget target_;
            std::string highest_timestamp_seen_;

            //methods
            virtual void saveSettings() = 0;
            virtual std::string requestResponse(const ScraperTarget& target) = 0;
            virtual std::vector<Comment> parseJSON(const rapidjson::Document& document) = 0;    // implementation is specific to derived class as the DOM varies from site to site.
            virtual std::vector<Comment> cleanComments(std::vector<Comment>& comments) = 0;

            //helpers
            utility::string_t stripBase64(const utility::string_t& s);
            std::string trim(const std::string& str);
            bool compareBigNumbers(const std::string& a, const std::string& b);
            virtual uint64_t getTime();
        public:
            
            //public methods

            /*
            @author: Gavin Frazar
            Parameters: none
            Return values: A vector of "Comment" objects. Comment is a simple key/value struct which holds two strings. One string to hold the contents of the comment, and another string to hold the ID of the user who posted the comment.
            Pre-conditions: The bot which uses this method must have a valid target to scrape data from. Its target social media site must be online and working. Watson Tone Analyzer must be working. Any rate limits must not have been exceeded on watson or
                            social media.
            Post-conditions: None. Any vector of comments returned is valid, even an empty vector of comments. It's entirely valid for there to be no comments returned by the search.
            Exceptions: This method will throw if any of the pre-conditions are not met. The following exceptions can be thrown:
                        -BadStatusException
            */
            virtual std::vector<Comment> getData();

            //While this method is public, it is also purely virtual, and therefore must be overridden in derived classes (which means documentation will be specific to the class overriding it).
            //For documentation of this method, see the overrides in derived classes.
            virtual void setSearchQuery(const std::string& query) = 0;

        }; //end of Bot abstract class
    }//end of Bot namespace
}//end of Apollo namespace
#endif