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

//returns true if a > b. Returns false if a <= b
bool Apollo::Bot::Bot::compareBigNumbers(const std::string & a, const std::string & b)
{
    if (a.size() != b.size())
        return a.size() > b.size(); //a and b have different number of digits,
                                    //both are unsigned integers -> greater value can be determined by checking who has more digits
    else
    {
        for (size_t i = 0; i < a.size(); ++i)
        {
            int a_num = a[i] - '0';
            int b_num = b[i] - '0';
            if (a_num != b_num)
                return a_num > b_num;
        }
        return false;  //a == b
    }
}

Apollo::Bot::Bot::Bot()
{
}

Apollo::Bot::Bot::~Bot()
{
}

std::vector<Apollo::Comment> Apollo::Bot::Bot::getData()
{
    std::vector<Comment> comments;
    if (target_.resource_url.size() == 0)
        throw Apollo::Bot::BadTargetException();

    std::string target_response;
    //send a request and receive a response
    try
    {
        target_response = requestResponse(target_);
    }
    catch(const Apollo::Bot::BadStatusException& e)
    {
        throw;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        throw;
    }

    //parse the response into a rapidjson Document
    rapidjson::Document target_document;
    target_document.Parse(target_response.c_str());

    //parse the JSON Document for "comments"
    std::vector<Comment> target_comments = parseJSON(target_document);

    //add target_comments to comments vector
    comments.insert(comments.end(), target_comments.begin(), target_comments.end());

    ////regex the comments to get all valid words (and ignore stopwords)
    this->saveSettings();
    return cleanComments(comments);
}
