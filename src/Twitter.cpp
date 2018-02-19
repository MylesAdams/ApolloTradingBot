#include "Twitter.h"

void Apollo::Bot::Twitter::saveSettings()
{
    using rapidjson::Value;
    std::ofstream file(resource_file_);
    rapidjson::Document doc;
    doc.SetObject();

    doc.AddMember("consumer_key", Value(this->consumer_key_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("consumer_secret", Value(this->consumer_secret_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("oauth_access_token_key", Value(this->oauth_access_token_key_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("oauth_access_token_secret", Value(this->oauth_access_token_secret_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("highest_timestamp_seen", Value(this->highest_timestamp_seen_), doc.GetAllocator());

    rapidjson::OStreamWrapper osw(file);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    doc.Accept(writer);
}

std::stringstream Apollo::Bot::Twitter::requestResponse(const std::string & target_url)
{
    std::stringstream response;


    return response;
}

Apollo::Bot::Twitter::Twitter()
{
    std::fstream file(resource_file_);

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        std::stringstream json;
        json << file.rdbuf();
        rapidjson::Document doc;
        doc.Parse(json.str().c_str());
        this->consumer_key_ = doc["consumer_key"].GetString();
        this->consumer_secret_ = doc["consumer_secret"].GetString();
        this->oauth_access_token_key_ = doc["oauth_access_token_key"].GetString();
        this->oauth_access_token_secret_ = doc["oauth_access_token_secret"].GetString();
        this->highest_timestamp_seen_ = std::atoi(doc["highest_timestamp_seen"].GetString());
    }
    else //create the JSON config file
    {
        this->consumer_key_ = "";
        this->consumer_secret_ = "";
        this->oauth_access_token_key_ = "";
        this->oauth_access_token_secret_ = "";
        this->highest_timestamp_seen_ = 0;
        this->saveSettings();
        throw std::runtime_error("Empty resource file -- /res/twitter.json does not contain keys, Twitter objected could not be constructed.");
    }
}

Apollo::Bot::Twitter::~Twitter()
{
    this->saveSettings();
}
