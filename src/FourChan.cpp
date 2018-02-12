#include "FourChan.h"

std::vector<Apollo::Comment> Apollo::Bot::FourChan::parseJSON(const rapidjson::Document & document)
{
    std::vector<Apollo::Comment> comments;
    for (int page_idx = 0; page_idx < document.Size(); ++page_idx)
    {
        const rapidjson::Value& threads = document[page_idx]["threads"];
        for (int thread_idx = 0; thread_idx < threads.Size(); ++thread_idx)
        {
            int thread_no = threads[thread_idx]["no"].GetInt();
            int replies = threads[thread_idx]["replies"].GetInt();
            if (replies > 0)
            {
                std::stringstream thread_response = requestResponse(this->INCOMPLETE_URLS_[0] + "/thread/" + std::to_string(thread_no) + ".json");

                rapidjson::Document thread;
                thread.Parse(thread_response.str().c_str());

                rapidjson::Value &posts = thread["posts"];
                for (int com_idx = 0; com_idx < posts.Size(); ++com_idx)
                {
                    rapidjson::Value &comment = posts[com_idx];
                    if (comment.HasMember("com"))
                    {
                        comments.push_back(Comment(comment["com"].GetString(), std::to_string(comment["no"].GetInt())));
                    }
                }
            }
        }
    }
    return comments;
}

Apollo::Bot::FourChan::FourChan(const std::vector<std::string>& complete_urls)
    : Bot(complete_urls)
{
}

Apollo::Bot::FourChan::FourChan(const std::vector<std::string>& complete_urls, const std::vector<std::string>& incomplete_urls)
    :   Bot(complete_urls, incomplete_urls)
{}
