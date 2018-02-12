#include "FourChan.h"

void Apollo::Bot::FourChan::saveSettings()
{
    std::ofstream out(this->resource_file);
    if (out.is_open())
    {
        std::stringstream ss;
        ss << this->highest_post_seen_;
        out << ss.rdbuf();
    }
}

void Apollo::Bot::FourChan::initFourChan()
{
    this->highest_post_seen_ = 0;
    std::ifstream res;
    res.open(this->resource_file);
    if (res.is_open())
    {
        std::stringstream ss;
        ss << res.rdbuf();
        unsigned long long num;
        if (ss >> num)
            if (num > highest_post_seen_)
                highest_post_seen_ = num;
    }
}

std::vector<Apollo::Comment> Apollo::Bot::FourChan::parseJSON(const rapidjson::Document & document)
{
    unsigned long long temp_highest_post_seen = this->highest_post_seen_;
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
                        unsigned long long post_no = comment["no"].GetInt();
                        if (post_no > this->highest_post_seen_)
                        {
                            comments.push_back(Comment(comment["com"].GetString(), std::to_string(post_no)));
                            if (post_no > temp_highest_post_seen)
                                temp_highest_post_seen = post_no;
                        }
                    }
                }
            }
        }
    }
    this->highest_post_seen_ = temp_highest_post_seen;
    return comments;
}

Apollo::Bot::FourChan::FourChan(const std::vector<std::string>& complete_urls)
    : Bot(complete_urls)
{
    this->initFourChan();
}

Apollo::Bot::FourChan::FourChan(const std::vector<std::string>& complete_urls, const std::vector<std::string>& incomplete_urls)
    : Bot(complete_urls, incomplete_urls)
{
    this->initFourChan();
}

Apollo::Bot::FourChan::~FourChan()
{
    this->saveSettings();
}
