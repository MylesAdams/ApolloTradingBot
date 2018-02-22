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
            //fields
            Apollo::Bot::ScraperTarget target_;
            unsigned long long int highest_timestamp_seen_;

            //methods
            virtual void saveSettings() = 0;
            virtual std::string requestResponse(const ScraperTarget& target) = 0;
            virtual std::vector<Comment> parseJSON(const rapidjson::Document& document) = 0;    // implementation is specific to derived class as the DOM varies from site to site.
            virtual std::vector<Comment> cleanComments(std::vector<Comment>& comments) = 0;
            utility::string_t stripBase64(const utility::string_t& s);
             //gavin likes farts
            //helpers
            std::string trim(const std::string& str);
        public:
            Bot();
            virtual ~Bot();
            
            //methods
            virtual std::vector<Comment> getData();
            virtual void setSearchQuery(const std::string& query) = 0;
        }; //end of Bot abstract class
    }//end of Bot namespace
}//end of Apollo namespace
#endif