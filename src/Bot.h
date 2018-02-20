#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>

#include "Comment.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

namespace Apollo {
    namespace Bot { // Put all classes that extend Bot in the Bot namespace
        class Bot
        {
        private:
        protected:
            //fields
            //complete_urls -- valid urls that are immediately accessible without concatenating anything to them. You will need at least one of these.
            //incomple_urls -- base urls that must have things concatenated to them in order to be a valid url
            std::vector<std::string> complete_urls_;
            std::vector<std::string> incomplete_urls_;
            //methods
            virtual void saveSettings() = 0;
            virtual std::stringstream requestResponse(const std::string& target_url);
            virtual std::vector<Comment> parseJSON(const rapidjson::Document& document) = 0;    // implementation is specific to derived class as the DOM varies from site to site.
            virtual std::vector<Comment> cleanComments(std::vector<Comment>& comments) = 0;

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