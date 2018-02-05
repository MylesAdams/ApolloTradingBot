#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <string>
#include <vector>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Multi.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>


namespace Bot {
    class Bot
    {
    private:
    protected:
        //fields
        //TODO: declare a curlpp version of a libcurl multi-handle here
        std::vector<std::string> url_targets_;
        std::vector<curlpp::Easy> easy_handles_;

        //methods
        virtual std::string cleanData(std::stringstream &response) = 0;    //must be overridden in derived class
    public:
        //ctors
        
        //methods

    };
}

#endif