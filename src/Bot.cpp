#include "Bot.h"

Bot::Bot()
    : TARGET_URL_("test")
{}

Bot::Bot(std::string target_url)
    :   TARGET_URL_(target_url)
{
    this->easy_handle_.setOpt(cURLpp::Options::Url(this->TARGET_URL_));
}
