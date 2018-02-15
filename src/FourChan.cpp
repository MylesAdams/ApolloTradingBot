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
    return this->cleanComments(comments);
}

std::vector<Apollo::Comment> Apollo::Bot::FourChan::cleanComments(std::vector<Comment>& comments)
{
    //regex for removing post# quotes in 4chan replies
    std::regex rgx_linebreak("(<br>)+");
    std::regex rgx_misc("(&gt;)+|(<span class=\"quote\">)+|(</span>)+|(&quot;)+|(&#[0-9]+;)+|(/biz/thread/[0-9]+)+|(<wbr>)+|(https?://)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)");
    std::regex rgx_quotelink("(<a href=\".*\" class=\"quotelink\">[0-9]+</a>)");
    for (auto& comment : comments)
    {
        comment.content = std::regex_replace(comment.content, rgx_linebreak, " ");
        comment.content = std::regex_replace(comment.content, rgx_misc, "");
        comment.content = std::regex_replace(comment.content, rgx_quotelink, "");
        comment.content = this->trim(comment.content);
    }
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
