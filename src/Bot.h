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
    const std::string TARGET_URL_;
    curlpp::Easy easy_handle_;
public:
    //ctors
    Bot();
    Bot(std::string target_url);

    //methods

};

#endif