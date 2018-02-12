#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "Comment.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace Apollo {
    namespace Bot { // Put all classes that extend Bot in the Bot namespace
        class Bot
        {
        private:
        protected:
            //fields
            const std::vector<std::string> target_urls_;
            //methods
            virtual std::stringstream requestResponse(const std::string& target_url);
            virtual std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) = 0;    // implementation is specific to derived class as the DOM varies from site to site.
            virtual std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments);
        public:
            //ctors
            Bot(std::vector<std::string> target_urls);
            
            //methods
            virtual std::vector<Comment> getData();
        }; //end of Bot abstract class
    }//end of Bot namespace
}//end of Apollo namespace
#endif