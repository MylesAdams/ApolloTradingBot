#include "Reddit.h"

Apollo::Bot::Reddit::Reddit()
{
    this->INCOMPLETE_URLS_.push_back("https://reddit.com/r/");

}

Apollo::Bot::Reddit::Reddit(std::string subreddit)
{
    Reddit();
    this->COMPLETE_URLS_.push_back("https://reddit.com/r/" + subreddit + "new.json");
}
