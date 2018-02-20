#include "Bot.h"

Apollo::Bot::Bot::Bot()
{
}

std::stringstream Apollo::Bot::Bot::requestResponse(const std::string & resource_url, const std::string & request_path)
{
    std::stringstream response;
    http_client client(utility::conversions::to_string_t(resource_url));
    http_request req(methods::GET);
    const std::string method = "GET";

    req.headers() = this->getHeaders();

    // wait for all the outstanding i/o to complete and handle any exceptions.
    try {

        // Send https request.
        pplx::task<http_response> accounts_request = my_client.request(req);
        accounts_request

            // Hook up coninuation on response.
            .then([](http_response response) {

            // Throw on bad server response.
            if (response.status_code() != status_codes::OK) {
                throw std::exception("Received status code: " + response.status_code());
            }

            // Else extract json object.
            return response.extract_string();
        })

            // Continuation on extracted json.
            .then([&response](pplx::task<std::wstring> s) {

            // Process json object.
            std::string test = utility::conversions::s.get();
        }).wait(); // Wait for task group to complete.	
    }

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
    for (auto& complete_url : this->COMPLETE_URLS_)
    {
        //send a request and receive a response
        std::stringstream target_response = requestResponse(complete_url, "asdf");

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
