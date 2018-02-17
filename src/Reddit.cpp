#include "Reddit.h"

Apollo::Bot::Reddit::Reddit()
{
    this->INCOMPLETE_URLS_.push_back("https://reddit.com/r/");

}

Apollo::Bot::Reddit::Reddit(std::string subreddit)
{
    Reddit();
    this->COMPLETE_URLS_.push_back("https://reddit.com/r/" + subreddit + "new.json");
    this->COMPLETE_URLS_REDDIT_COUNT.push_back("https://reddit.com/r/" + subreddit + "about.json");
}

std::vector<Apollo::Comment> Apollo::Bot::Reddit::parseJSON(const rapidjson::Document &document)
{
    //need to read every comment. Should be called "self-text" in the json
    //should setup a way to check what comment has been read most recently
    //so that I don't read one's that have already been read
    std::vector<Apollo::Comment> comments;
    return std::vector<Apollo::Comment>();
}

std::vector<Apollo::Comment> Apollo::Bot::Reddit::cleanComments(std::vector<Apollo::Comment> &comments)
{
    //comments still contain some degree of html or other markup languages, so cleaning them up will be nice
    //probably just a few regex lines should do
    return std::vector<Apollo::Comment>();
}

std::vector<std::string> Apollo::Bot::Reddit::parseJSONSubscriberCount(const rapidjson::Document &document)
{
    //need to read just the subscriber count from the COMPLETE_URLS_REDDIT_COUNT
    return std::vector<std::string>();
}

Apollo::Bot::Reddit::~Reddit() {
    //something happens
}
