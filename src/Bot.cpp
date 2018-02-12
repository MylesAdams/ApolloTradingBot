#include "Bot.h"

std::stringstream Apollo::Bot::Bot::requestResponse(const std::string & target_url)
{
    std::stringstream response;
    curlpp::Cleanup cleaner; // curlpp's RAII -- not really sure if this is deprecated but all examples still use it
    curlpp::Easy easy_request;
    easy_request.setOpt(curlpp::Options::Url(target_url));
    easy_request.setOpt(curlpp::options::WriteStream(&response));
    easy_request.perform(); // TODO add exception handling -- this can throw

    return response;
}

std::vector<Apollo::Comment> Apollo::Bot::Bot::cleanComments(std::vector<Comment>& comments)
{
    //regex to strip all unnecessary content (e.g. html tags)
    for (auto& comment : comments)
    {
        //strip HTML
        //PLACEHOLDER

        //remove stopwords
        //PLACEHOLDER
    }

    return comments;
}

Apollo::Bot::Bot::Bot(const std::vector<std::string>& complete_urls)
    : COMPLETE_URLS_(complete_urls)
{}

Apollo::Bot::Bot::Bot(const std::vector<std::string>& complete_urls, const std::vector<std::string>& incomplete_urls)
    :   COMPLETE_URLS_(complete_urls),
        INCOMPLETE_URLS_(incomplete_urls)
{}

std::vector<Apollo::Comment> Apollo::Bot::Bot::getData()
{
    std::vector<Comment> comments;
    for (auto& complete_url : this->COMPLETE_URLS_)
    {
        //send a request and receive a response
        std::stringstream target_response = requestResponse(complete_url);

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
