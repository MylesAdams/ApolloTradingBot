/*
4chan example of using curlpp and rapidjson
*/

#include "FourChan.h"

#include <iostream>
#include <sstream>
#include <cctype>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using Apollo::Comment;

int main()
{
    using rapidjson::Value;
    std::ofstream file("../res/test.json");
    rapidjson::Document doc;
    doc.SetObject();
    std::string consumer_key_ = "ckey";
    std::string  consumer_secret_ = "csecret";
    std::string oauth_access_token_key_ = "okey";
    std::string  oauth_access_token_secret_ = "osecret";
    unsigned long long testnum = 5;
    doc.AddMember("testnum", Value(testnum), doc.GetAllocator());
    doc.AddMember("consumer_key", Value(consumer_key_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("consumer_secret", Value(consumer_secret_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("oauth_access_token_key", Value(oauth_access_token_key_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("oauth_access_token_secret", Value(oauth_access_token_secret_, doc.GetAllocator()), doc.GetAllocator());

    rapidjson::OStreamWrapper osw(file);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    doc.Accept(writer);
    file.close();
    return 0;
}