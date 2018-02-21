#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>
#include <algorithm>

#include "Comment.h"

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

        struct RequestParameter //TODO -- make this into a class with a .h and .cpp file. Literally make it exactly the same but as a class.
        {
            utility::string_t key;
            utility::string_t value;
            RequestParameter(const utility::string_t& key, const utility::string_t& value)
                : key(key), value(value)
            {
            }
            bool operator < (const RequestParameter& str) const
            {
                return (key < str.key);
            }
        };

        struct ScraperTarget //TODO -- make this into a class with a .h and .cpp file. Literally make it exactly the same but as a class.
        {
            const utility::string_t resource_url;
            const utility::string_t request_path;
            std::vector<RequestParameter> request_parameters;
            ScraperTarget(const utility::string_t& resource_url, const utility::string_t& request_path)
                : resource_url(resource_url),
                request_path(request_path) {}
        };

        class Bot
        {
        private:
        protected:
            //fields
            std::vector<ScraperTarget> targets_;

            //methods
            virtual void saveSettings() = 0;
            virtual std::stringstream requestResponse(const ScraperTarget& target) = 0;
            virtual std::vector<Comment> parseJSON(const rapidjson::Document& document) = 0;    // implementation is specific to derived class as the DOM varies from site to site.
            virtual std::vector<Comment> cleanComments(std::vector<Comment>& comments) = 0;
            utility::string_t stripBase64(const utility::string_t& s);
            //helpers
            std::string trim(const std::string& str);
        public:
            virtual ~Bot();
            
            //methods
            virtual std::vector<Comment> getData();
        }; //end of Bot abstract class
    }//end of Bot namespace
}//end of Apollo namespace
#endif