#include "FourChan.h"

//void Apollo::Bot::FourChan::saveSettings()
//{
//    std::ofstream out(this->resource_file_);
//    if (out.is_open())
//    {
//        std::stringstream ss;
//        ss << this->highest_post_seen_;
//        ss << " ";
//        ss << this->highest_timestamp_seen_;
//        out << ss.rdbuf();
//    }
//}
//
//std::vector<Apollo::Comment> Apollo::Bot::FourChan::parseJSON(const rapidjson::Document & document)
//{
//    unsigned long long temp_highest_post_seen = this->highest_post_seen_;
//    unsigned long long temp_highest_timestamp_seen = this->highest_timestamp_seen_;
//    std::vector<Apollo::Comment> comments;
//    //for (int page_idx = 0; page_idx < document.Size(); ++page_idx)
//    //{
//    //    const rapidjson::Value& threads = document[page_idx]["threads"];
//    //    for (int thread_idx = 0; thread_idx < threads.Size(); ++thread_idx)
//    //    {
//    //        unsigned long long int thread_no = threads[thread_idx]["no"].GetInt();
//    //        unsigned long long int last_modified = threads[thread_idx]["last_modified"].GetInt();
//    //        if (last_modified > this->highest_timestamp_seen_)
//    //        {
//    //            if (last_modified > temp_highest_timestamp_seen)
//    //                temp_highest_timestamp_seen = last_modified;
//
//    //            std::stringstream thread_response = requestResponse(this->incomplete_urls_[0] + "/thread/" + std::to_string(thread_no) + ".json", "placeholder");
//    //            rapidjson::Document thread;
//    //            thread.Parse(thread_response.str().c_str());
//
//    //            rapidjson::Value &posts = thread["posts"];
//    //            for (int com_idx = 0; com_idx < posts.Size(); ++com_idx)
//    //            {
//    //                rapidjson::Value &comment = posts[com_idx];
//    //                if (comment.HasMember("com"))
//    //                {
//    //                    unsigned long long post_no = comment["no"].GetInt();
//    //                    if (post_no > this->highest_post_seen_)
//    //                    {
//    //                        comments.push_back(Comment(comment["com"].GetString(), std::to_string(post_no)));
//    //                        if (post_no > temp_highest_post_seen)
//    //                            temp_highest_post_seen = post_no;
//    //                    }
//    //                }
//    //            }
//    //        }
//    //    }
//    //}
//    //this->highest_post_seen_ = temp_highest_post_seen;
//    //this->highest_timestamp_seen_ = temp_highest_timestamp_seen;
//    return this->cleanComments(comments);
//}
//
//std::vector<Apollo::Comment> Apollo::Bot::FourChan::cleanComments(std::vector<Comment>& comments)
//{
//    //regex for removing post# quotes in 4chan replies
//    std::regex rgx_linebreak("(<br>)+");
//    std::regex rgx_misc("(&gt;)+|(<span class=\"quote\">)+|(</span>)+|(&quot;)+|(&#[0-9]+;)+|(/biz/thread/[0-9]+)+|(<wbr>)+|(https?://)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)");
//    std::regex rgx_quotelink("(<a href=\".*\" class=\"quotelink\">[0-9]+</a>)");
//    for (auto& comment : comments)
//    {
//        comment.content = std::regex_replace(comment.content, rgx_linebreak, " ");
//        comment.content = std::regex_replace(comment.content, rgx_misc, "");
//        comment.content = std::regex_replace(comment.content, rgx_quotelink, "");
//        comment.content = this->trim(comment.content);
//        std::transform(comment.content.begin(), comment.content.end(), comment.content.begin(), ::tolower);
//    }
//    return comments;
//}
//
//Apollo::Bot::FourChan::FourChan()
//{
//    this->complete_urls_.push_back("https://a.4cdn.org/biz/threads.json");
//    this->incomplete_urls_.push_back("https://a.4cdn.org/biz");
//    this->highest_post_seen_ = 0;
//    this->highest_timestamp_seen_ = 0;
//    std::ifstream res;
//    res.open(this->resource_file_);
//    if (res.is_open())
//    {
//        std::stringstream ss;
//        ss << res.rdbuf();
//        unsigned long long num;
//        if (ss >> num)
//            if (num > highest_post_seen_)
//                highest_post_seen_ = num;
//        if (ss >> num)
//            if (num > highest_timestamp_seen_)
//                highest_timestamp_seen_ = num;
//    }
//}
//
//Apollo::Bot::FourChan::~FourChan()
//{
//    this->saveSettings();
//}
