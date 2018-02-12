/*
4chan example of using curlpp and rapidjson
*/



#include <iostream>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

int main()
{
    const std::string BASE_URL_ = "https://a.4cdn.org/biz/";
    curlpp::Cleanup cleaner;
    curlpp::Easy my_request;
    std::stringstream json_response;

    my_request.setOpt(curlpp::Options::Url(BASE_URL_ + "catalog.json"));
    my_request.setOpt(curlpp::options::WriteStream(&json_response));
    my_request.perform();
    
    // 1. Parse a JSON string into DOM.
    rapidjson::Document doc;
    doc.Parse(json_response.str().c_str());

    for (int page_idx = 0; page_idx < doc.Size(); ++page_idx)
    {
        rapidjson::Value &threads = doc[page_idx]["threads"];
        for (int thread_idx = 0; thread_idx < threads.Size(); ++thread_idx)
        {
            int thread_no = threads[thread_idx]["no"].GetInt();
            int replies = threads[thread_idx]["replies"].GetInt();
            if (replies > 0)
            {
                curlpp::Easy thread_request;
                thread_request.setOpt(curlpp::Options::Url(BASE_URL_ + "/thread/" + std::to_string(thread_no) + ".json"));
                std::stringstream thread_response;
                thread_request.setOpt(curlpp::options::WriteStream(&thread_response));
                thread_request.perform();

                rapidjson::Document thread;
                thread.Parse(thread_response.str().c_str());
                
                rapidjson::Value &posts = thread["posts"];
                for (int com_idx = 0; com_idx < posts.Size(); ++com_idx)
                {
                    std::cout << "\nFETCHING NEXT COMMENT FROM THREAD# " << thread_no << "\n---------------------------------------------------\n" << std::endl;
                    rapidjson::Value &comment = posts[com_idx];
                    if (comment.HasMember("com"))
                        std::cout << comment["com"].GetString() << std::endl;
                }
            }
        }
    }
    return 0;
}