#include "FourChan.h"

void Apollo::Bot::FourChan::saveSettings()
{
    std::ofstream out(this->RESOURCE_FILE_);
    if (out.is_open())
    {
        std::stringstream ss;
        ss << this->highest_post_seen_;
        ss << " ";
        ss << this->highest_timestamp_seen_;
        out << ss.rdbuf();
    }
}

std::string Apollo::Bot::FourChan::requestResponse(const ScraperTarget & target)
{
    // Namespace.
    using namespace utility;                    // Common utilities like string conversions.
    using namespace web;                        // Common features like URIs.
    using namespace web::http;                  // Common HTTP functionality.
    using namespace web::http::client;          // HTTP client features.
    using namespace concurrency::streams;		// Asynchronous streams.
    using utility::string_t;

    const string_t METHOD(U("GET"));
    const string_t CONTENT_TYPE(U("application/json"));

    http_request req(methods::GET);
    req.headers().set_content_type(CONTENT_TYPE);
    uri_builder builder(target.REQUEST_PATH);
    req.set_request_uri(builder.to_string());

    std::string response;
    http_client client(this->BASE_URL_);
    client.request(req)
        .then([](http_response res)
        {
            return res.extract_utf8string();
        })
        .then([&response](pplx::task<std::string> utf8str)
        {
            response = utf8str.get();
        }).wait();

    return response;
}

std::vector<Apollo::Comment> Apollo::Bot::FourChan::parseJSON(const rapidjson::Document & document)
{
    unsigned long long temp_highest_post_seen = this->highest_post_seen_;
    unsigned long long temp_highest_timestamp_seen = this->highest_timestamp_seen_;
    std::vector<Apollo::Comment> comments;
    for (int page_idx = 0; page_idx < document.Size(); ++page_idx)
    {
        const rapidjson::Value& threads = document[page_idx]["threads"];
        for (int thread_idx = 0; thread_idx < threads.Size(); ++thread_idx)
        {
            unsigned long long int thread_no = threads[thread_idx]["no"].GetInt();
            unsigned long long int last_modified = threads[thread_idx]["last_modified"].GetInt();
            if (last_modified > this->highest_timestamp_seen_)
            {
                if (last_modified > temp_highest_timestamp_seen)
                    temp_highest_timestamp_seen = last_modified;

                utility::string_t thread_request_path = U("/biz/thread/") + utility::conversions::to_string_t(std::to_string(thread_no)) + U(".json");
                std::string thread_response = requestResponse(ScraperTarget(this->BASE_URL_, thread_request_path));
                rapidjson::Document thread;
                thread.Parse(thread_response.c_str());

                rapidjson::Value &posts = thread["posts"];
                for (int com_idx = 0; com_idx < posts.Size(); ++com_idx)
                {
                    rapidjson::Value &comment = posts[com_idx];
                    if (comment.HasMember("com"))
                    {
                        unsigned long long post_no = comment["no"].GetInt();
                        if (post_no > this->highest_post_seen_)
                        {
                            std::string comment_content = comment["com"].GetString();
                            //put to lower case
                            std::transform(comment_content.begin(), comment_content.end(), comment_content.begin(), ::tolower);
                            bool matched_pattern = false;
                            for (const auto& pattern : this->search_patterns_)
                            {
                                if (comment_content.find(pattern) != std::string::npos)
                                {
                                    matched_pattern = true;
                                    break;
                                }
                            }
                            if (matched_pattern)
                                comments.push_back(Comment(comment_content, std::to_string(post_no)));
                            if (post_no > temp_highest_post_seen)
                                temp_highest_post_seen = post_no;
                        }
                    }
                }
            }
        }
    }
    this->highest_post_seen_ = temp_highest_post_seen;
    this->highest_timestamp_seen_ = temp_highest_timestamp_seen;
    return this->cleanComments(comments);
}

std::vector<Apollo::Comment> Apollo::Bot::FourChan::cleanComments(std::vector<Comment>& comments)
{
    //took out regex for now. Regex was too slow.
    for (auto& comment : comments)
    {
        comment.content = this->trim(comment.content);
    }
    return comments;
}

Apollo::Bot::FourChan::FourChan()
{
    this->highest_post_seen_ = 0;
    this->highest_timestamp_seen_ = 0;
    ScraperTarget target(this->BASE_URL_, U("/biz/threads.json"));
    this->targets_ = target;

    std::ifstream res;
    res.open(this->RESOURCE_FILE_);
    if (res.is_open())
    {
        std::stringstream ss;
        ss << res.rdbuf();
        unsigned long long num;
        if (ss >> num)
            if (num > highest_post_seen_)
                highest_post_seen_ = num;
        if (ss >> num)
            if (num > highest_timestamp_seen_)
                highest_timestamp_seen_ = num;
    }
}

Apollo::Bot::FourChan::~FourChan()
{
    this->saveSettings();
}

void Apollo::Bot::FourChan::setSearchQuery(const std::string & query)
{
    std::string q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);
    this->search_patterns_.push_back(q);
}
