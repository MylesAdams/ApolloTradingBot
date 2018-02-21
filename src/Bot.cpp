#include "Bot.h"

utility::string_t Apollo::Bot::Bot::stripBase64(const utility::string_t & s)
{
    std::vector<unsigned char> buffer;
    buffer.reserve(s.size());
    for (unsigned int i = 0; i < s.size(); ++i)
        if (isalnum(s[i]))
            buffer.push_back(s[i]);

    utility::string_t stripped_string(buffer.begin(), buffer.end());
    return stripped_string;
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
    for (auto& target : this->targets_)
    {
        //send a request and receive a response
        std::stringstream target_response = requestResponse(target);

        //parse the response into a rapidjson Document
        rapidjson::Document target_document;
        target_document.Parse(target_response.str().c_str());

        //parse the JSON Document for "comments"
        std::vector<Comment> target_comments = parseJSON(target_document);

        //add target_comments to comments vector
        comments.insert(comments.end(), target_comments.begin(), target_comments.end());
    }

    ////regex the comments to get all valid words (and ignore stopwords)

    return cleanComments(comments);
}
