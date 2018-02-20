#include "Reddit.h"

Apollo::Bot::Reddit::Reddit()
{
    this->INCOMPLETE_URLS_.push_back("https://reddit.com/r/");

}

Apollo::Bot::Reddit::Reddit(std::string subreddit)
{
    Reddit();
    this->COMPLETE_URLS_.push_back("http://reddit.com/r/" + subreddit + "/comments.json");
    this->COMPLETE_URLS_REDDIT_COUNT.push_back("https://reddit.com/r/" + subreddit + "about.json");
}

std::vector<Apollo::Comment> Apollo::Bot::Reddit::parseJSON(const rapidjson::Document &document) {

    //need to read every comment. Should be called "body" in the json
    //should setup a way to check what comment has been read most recently
    //comments have a creation time. So mark the most recent one and then read up to it each time
    //replacing the num with the newest one
    //so that I don't read one's that have already been read
    std::vector<Apollo::Comment> comments;
    //const rapidjson::Value &children = document["children"];

    //unsigned long int temp_last_comment_read = children[0]["created"].GetUint();

    for (int index = 0; index < 2; index++) {
        Comment redditComment;
        redditComment.content;
        const rapidjson::Value &one = document["data"];
        //auto one = document["data"];
        //auto two = one["children"][index]["data"]["body"].GetString();
        comments.push_back(redditComment);
    }
   /* for (int index = 0; index < 10; index++) {
        Comment redditComment;
        redditComment.content = document["data"]["children"][index]["data"]["body"].GetString();
        comments.push_back(redditComment);
    }*/

    /*for (auto& comment : children.GetArray())
    {
        if (children[comment]["created"].GetInt() == this->last_comment_read_)
        {
            this->last_comment_read_ = temp_last_comment_read;
            break;
        }

    //comments.push_back(Comment(children[comment]["body"].GetString()));


    }*/

    return comments;
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

Apollo::Bot::Reddit::~Reddit()
{
    //something happens
}
