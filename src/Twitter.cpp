#include "Twitter.h"

std::string Apollo::Bot::Twitter::percentEncode(const utility::string_t& s)
{
//    std::string str = utility::conversions::utf16_to_utf8(s);
    std::string str = utility::conversions::to_utf8string(s);
    std::vector<unsigned char> buffer;
    for (int i = 0; i < str.size(); ++i)
    {
        unsigned ch = str[i];
        if ((ch > 0x60 && ch < 0x7B) || (ch > 0x2F && ch < 0x3A) || (ch > 0x40 && ch < 0x5B) || ch == 0x2D || ch == 0x2E || ch == 0x5F || ch == 0x7E)   //twitter api doesn't want these character percent encoded
            buffer.push_back(ch);
        else
        {
            //convert to percent encoding, e.g.: 0xE6 -> "%E6"

            wchar_t LSB = ch % 0x10;    //extract least signicant hex 
            if (LSB >= 0x0A && LSB <= 0x0F) //extracted a hex number (a - f)
                LSB += 0x37;            //convert the char into the UTF8 encoding for the capital-cased letter it represents
            else                        //extracted a hex number (0 - 9)
                LSB += 0x30;            //convert char to UTF8 encoding for the number it represents
            ch >>= 0x04;               //shift ch right (divide by 16) for next extraction
            wchar_t MSB = ch % 0x10;    //extract the most significant hex
            if (MSB >= 0x0A && MSB <= 0x0F) //extracted a hex number (a - f)
                MSB += 0x37;            //convert the char into the UTF8 encoding for the capital-cased letter it represents
            else                        //extracted a hex number (0 - 9)
                MSB += 0x30;            //convert char to UTF8 encoding for the number it represents

            buffer.push_back(0x25); //add a '%' character, which is 0x25 in hex

            buffer.push_back(MSB);
            buffer.push_back(LSB);
        }
    }

    std::string encoded_str(buffer.begin(), buffer.end());
    return encoded_str;
}

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

std::stringstream Apollo::Bot::Twitter::requestResponse(const ScraperTarget& target)
{
    // Namespace.
    using namespace utility;                    // Common utilities like string conversions.
    using namespace web;                        // Common features like URIs.
    using namespace web::http;                  // Common HTTP functionality.
    using namespace web::http::client;          // HTTP client features.
    using namespace concurrency::streams;		// Asynchronous streams.

    //get target info
    const auto resource_url = target.resource_url;
    const auto request_path = target.request_path;
    std::vector<RequestParameter> request_parameters = target.request_parameters;

    // Get time since UNIX epoch
    uint64_t utc = datetime::utc_timestamp();

    std::vector<unsigned char> seed;
    for (int i = 0; i < 32; ++i)
        seed.push_back(utc % (31 * (i % 7 + 1)));   //generates 32 bytes of pseudo random data

    //encode===========
    const string_t oauth_consumer_key = utility::conversions::to_string_t(this->consumer_key_);
    const string_t oauth_nonce = this->stripBase64(utility::conversions::to_base64(seed)); //convert to base64 -> strip non-words -> set nonce
    const string_t oauth_signature_method = U("HMAC-SHA1");
    const string_t oauth_timestamp = utility::conversions::to_string_t(std::to_string(utc));
    const string_t oauth_token = utility::conversions::to_string_t(this->oauth_access_token_key_);
    const string_t oauth_version = U("1.0");
    //=============this

    request_parameters.push_back(RequestParameter(U("oauth_consumer_key"), oauth_consumer_key));
    request_parameters.push_back(RequestParameter(U("oauth_nonce"), oauth_nonce));
    request_parameters.push_back(RequestParameter(U("oauth_signature_method"), oauth_signature_method));
    request_parameters.push_back(RequestParameter(U("oauth_timestamp"), oauth_timestamp));
    request_parameters.push_back(RequestParameter(U("oauth_token"), oauth_token));
    request_parameters.push_back(RequestParameter(U("oauth_version"), oauth_version));

    std::sort(request_parameters.begin(), request_parameters.end());

    std::string parameter_string;
    for (const auto& param : request_parameters)
        parameter_string.append(percentEncode(param.key) + '=' + percentEncode(param.value) + '&');
    parameter_string.pop_back(); // removes the last '&'

    //create output string
    const std::string method = "GET";
    const std::string signature_base_string = method + "&" + percentEncode(resource_url + request_path) + "&" + percentEncode(utility::conversions::to_string_t(parameter_string));

    //create signing key
    const std::string signing_key = percentEncode(utility::conversions::to_string_t(this->consumer_secret_)) + "&" + percentEncode(utility::conversions::to_string_t(this->oauth_access_token_secret_));

    //convert to stuff HMAC will take
    unsigned char* signature_base_array = new unsigned char[signature_base_string.size()];
    for (size_t i = 0; i < signature_base_string.size(); ++i)
        signature_base_array[i] = signature_base_string[i];

    unsigned char signature_bytes[EVP_MAX_MD_SIZE];
    unsigned int signature_bytes_length;
    HMAC(EVP_sha1(), signing_key.c_str(), signing_key.size(), signature_base_array, signature_base_string.size(), signature_bytes, &signature_bytes_length);

    //convert from array of bytes to vector of bytes
    std::vector<unsigned char> signature_vector;
    for (size_t i = 0; i < signature_bytes_length; ++i)
        signature_vector.push_back(signature_bytes[i]);

    utility::string_t oauth_signature = utility::conversions::to_base64(signature_vector);

    //free allocated memory
    delete[] signature_base_array;

    // Create client.
    http_client my_client(resource_url);

    // Declare request.
    http_request req(methods::GET);
    const string_t content_type = U("application/json");
    req.headers().set_content_type(content_type);								// Sets content type to application/json.
    uri_builder builder(request_path);
    for (auto& param : target.request_parameters)
        builder.append_query(param.key, param.value);
    //req.set_request_uri(request_path + U("?count=5&screen_name=vechainofficial"));
    req.set_request_uri(builder.to_string());

    //build the authorization header
    auto authorization_header = "OAuth " + percentEncode(U("oauth_consumer_key")) + "=\"" + percentEncode(oauth_consumer_key) + "\", "
        + percentEncode(U("oauth_nonce")) + "=\"" + percentEncode(oauth_nonce) + "\", "
        + percentEncode(U("oauth_signature")) + "=\"" + percentEncode(oauth_signature) + "\", "
        + percentEncode(U("oauth_signature_method")) + "=\"" + percentEncode(oauth_signature_method) + "\", "
        + percentEncode(U("oauth_timestamp")) + "=\"" + percentEncode(oauth_timestamp) + "\", "
        + percentEncode(U("oauth_token")) + "=\"" + percentEncode(oauth_token) + "\", "
        + percentEncode(U("oauth_version")) + "=\"" + percentEncode(oauth_version) + "\"";

    req.headers().add(U("Accept"), U("/*/"));
    req.headers().add(U("Connection"), U("close"));
    req.headers().add(U("Authorization"), utility::conversions::to_string_t(authorization_header));
    std::stringstream response;
    try {

        // Send https request.
        pplx::task<http_response> my_request = my_client.request(req);
        my_request.then([](http_response res)
        {
            auto stat = res.status_code();
            std::cout << "Status code:\t" << stat << std::endl;
            return res.extract_vector();
        }).then([&response](pplx::task<std::vector<unsigned char>> vector_task)
        {
            auto v = vector_task.get();
            std::string str(v.begin(), v.end());
            response << str;
        }).wait(); // Wait for task group to complete.	
    }
    catch (...) {}

    return response;
}

std::vector<Apollo::Comment> Apollo::Bot::Twitter::parseJSON(const rapidjson::Document & document)
{
    std::vector<Comment> comments;

    for (size_t i = 0; i < document.Size(); ++i)
    {
        comments.push_back(Comment(document[i]["full_text"].GetString(), "placeholder"));
    }
    return comments;
}

std::vector<Apollo::Comment> Apollo::Bot::Twitter::cleanComments(std::vector<Comment>& comments)
{
    return comments;
}

Apollo::Bot::Twitter::Twitter()
{
    std::ifstream file(resource_file_);

    if (file.peek() != std::ifstream::traits_type::eof())
    {
        std::stringstream json;
        json << file.rdbuf();
        rapidjson::Document doc;
        doc.Parse(json.str().c_str());
        this->consumer_key_ = doc["consumer_key"].GetString();
        this->consumer_secret_ = doc["consumer_secret"].GetString();
        this->oauth_access_token_key_ = doc["oauth_access_token_key"].GetString();
        this->oauth_access_token_secret_ = doc["oauth_access_token_secret"].GetString();
        this->highest_timestamp_seen_ = doc["highest_timestamp_seen"].GetUint64();
        //TODO implement a way for a user of this class to add timelines to track (screen_name), and max number of tweets to get (count).
        ScraperTarget vechain(U("https://api.twitter.com"), U("/1.1/statuses/user_timeline.json"));
        vechain.request_parameters.push_back(RequestParameter(U("count"), U("10")));
        vechain.request_parameters.push_back(RequestParameter(U("screen_name"), U("vechainofficial")));
        vechain.request_parameters.push_back(RequestParameter(U("tweet_mode"), U("extended")));
        this->targets_.push_back(vechain);
    }
    else //create the JSON config file
    {
        this->consumer_key_ = "";
        this->consumer_secret_ = "";
        this->oauth_access_token_key_ = "";
        this->oauth_access_token_secret_ = "";
        this->highest_timestamp_seen_ = 0;
        this->saveSettings();
        throw std::runtime_error("Empty resource file -- /res/twitter.json does not contain keys, Twitter object could not be constructed.");
    }
}

Apollo::Bot::Twitter::~Twitter()
{
    this->saveSettings();
}
