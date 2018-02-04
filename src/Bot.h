#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <string>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

class Bot
{
private:
protected:
    //fields
    const std::string TARGET_URL_;
    curlpp::Easy easy_handle_;

    //methods
    virtual std::string cleanInput(std::stringstream &response) = 0;    //must be overridden in derived class
public:
    //ctors
    Bot();
    Bot(std::string target_url);

    //methods

};

#endif