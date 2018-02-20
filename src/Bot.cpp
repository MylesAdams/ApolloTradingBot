#include "Bot.h"

std::stringstream Apollo::Bot::Bot::requestResponse(const std::string & resource_url, const std::string& request_path)
{
    std::stringstream response;


    return response;
}

std::string Apollo::Bot::Bot::trim(const std::string & str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

Apollo::Bot::Bot::~Bot()
{
}

std::vector<Apollo::Comment> Apollo::Bot::Bot::getData()
{
    std::vector<Comment> comments;
    for (auto& complete_url : this->complete_urls_)
    {
        //send a request and receive a response
        std::stringstream target_response = requestResponse(complete_url, "placeholder");

        //parse the response into a rapidjson Document
        rapidjson::Document target_document;
        target_document.Parse(target_response.str().c_str());

        //parse the JSON Document for "comments"
        std::vector<Comment> target_comments = parseJSON(target_document);

        //add target_comments to comments vector
        comments.insert(comments.end(), target_comments.begin(), target_comments.end());
    }

    //regex the comments to get all valid words (and ignore stopwords)
    return cleanComments(comments);
}
