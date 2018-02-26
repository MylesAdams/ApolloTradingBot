#include "Twitter.h"

std::string Apollo::Bot::Twitter::percentEncode(const utility::string_t& s)
{
    std::string str = utility::conversions::to_utf8string(s);
    std::vector<unsigned char> buffer;
    for (int i = 0; i < str.size(); ++i)
    {
        unsigned ch = str[i];
        if ((ch > 0x60 && ch < 0x7B) || (ch > 0x2F && ch < 0x3A) || (ch > 0x40 && ch < 0x5B) || ch == 0x2D || ch == 0x2E || ch == 0x5F || ch == 0x7E)   //twitter api doesn't want these characters percent encoded
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
    std::ofstream file(RESOURCE_FILE_);
    rapidjson::Document doc;
    doc.SetObject();

    doc.AddMember("consumer_key", Value(this->consumer_key_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("consumer_secret", Value(this->consumer_secret_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("oauth_access_token_key", Value(this->oauth_access_token_key_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("oauth_access_token_secret", Value(this->oauth_access_token_secret_, doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember("highest_timestamp_seen", Value(this->highest_timestamp_seen_, doc.GetAllocator()), doc.GetAllocator());

    rapidjson::OStreamWrapper osw(file);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    doc.Accept(writer);
}

std::string Apollo::Bot::Twitter::requestResponse(const ScraperTarget& target)
{
    // Namespace.
    using namespace utility;                    // Common utilities like string conversions.
    using namespace web;                        // Common features like URIs.
    using namespace web::http;                  // Common HTTP functionality.
    using namespace web::http::client;          // HTTP client features.
    using namespace concurrency::streams;		// Asynchronous streams.

    //get target info
    const auto RESOURCE_URL = target.resource_url;
    const auto REQUEST_PATH = target.request_path;
    auto request_parameters = target.request_parameters;

    // Get time since UNIX epoch
    uint64_t utc = this->getTime();

    //encode===========
    const string_t OAUTH_CONSUMER_KEY = utility::conversions::to_string_t(this->consumer_key_);
    const string_t OAUTH_NONCE = this->generateNonce(utc); //convert to base64 -> strip non-words -> set nonce
    const string_t OAUTH_SIGNATURE_METHOD = U("HMAC-SHA1");
    const string_t OAUTH_TIMESTAMP = utility::conversions::to_string_t(std::to_string(utc));
    const string_t OAUTH_TOKEN = utility::conversions::to_string_t(this->oauth_access_token_key_);
    const string_t OAUTH_VERSION = U("1.0");
    //=============this

    //this will benecessary to sort all of the parameters by key
    request_parameters.push_back(RequestParameter(U("oauth_consumer_key"), OAUTH_CONSUMER_KEY));
    request_parameters.push_back(RequestParameter(U("oauth_nonce"), OAUTH_NONCE));
    request_parameters.push_back(RequestParameter(U("oauth_signature_method"), OAUTH_SIGNATURE_METHOD));
    request_parameters.push_back(RequestParameter(U("oauth_timestamp"), OAUTH_TIMESTAMP));
    request_parameters.push_back(RequestParameter(U("oauth_token"), OAUTH_TOKEN));
    request_parameters.push_back(RequestParameter(U("oauth_version"), OAUTH_VERSION));

    //sort parameters by key, as required by Twitter's API
    std::sort(request_parameters.begin(), request_parameters.end());

    std::string parameter_string;
    for (const auto& param : request_parameters)
        parameter_string.append(percentEncode(param.key) + '=' + percentEncode(param.value) + '&');
    parameter_string.pop_back(); // removes the last '&'
    
    //create the base string used for the signature
    const std::string METHOD = utility::conversions::to_utf8string(target.request_method);
    const std::string SIGNATURE_BASE_STRING = METHOD + "&" + percentEncode(RESOURCE_URL + REQUEST_PATH) + "&" + percentEncode(utility::conversions::to_string_t(parameter_string));

    //create the key that will be used to sign the base string, producing a signature
    const std::string SIGNING_KEY = percentEncode(utility::conversions::to_string_t(this->consumer_secret_)) + "&" + percentEncode(utility::conversions::to_string_t(this->oauth_access_token_secret_));

    //HMAC requires specific parameter types
    unsigned char* signature_base_array = new unsigned char[SIGNATURE_BASE_STRING.size()];
    for (size_t i = 0; i < SIGNATURE_BASE_STRING.size(); ++i)
        signature_base_array[i] = SIGNATURE_BASE_STRING[i];

    //create the signature with SHA1 encryption
    unsigned char signature_bytes[EVP_MAX_MD_SIZE];
    unsigned int signature_bytes_length;
    HMAC(EVP_sha1(), SIGNING_KEY.c_str(), SIGNING_KEY.size(), signature_base_array, SIGNATURE_BASE_STRING.size(), signature_bytes, &signature_bytes_length);

    //convert from array of bytes to vector of bytes
    std::vector<unsigned char> signature_vector;
    for (size_t i = 0; i < signature_bytes_length; ++i)
        signature_vector.push_back(signature_bytes[i]);

    //encode the signature in base64
    const utility::string_t OAUTH_SIGNATURE = utility::conversions::to_base64(signature_vector);
    this->checkSig(OAUTH_SIGNATURE);

    //free allocated memory
    delete[] signature_base_array;

    // Create client.
    http_client my_client(RESOURCE_URL);

    // Declare request.
    http_request req(target.request_method);
    req.headers().set_content_type(U("application/json")); // Sets content type to application/json.
   
    //build the request path
    uri_builder builder(REQUEST_PATH);
    for (auto& param : target.request_parameters)
        builder.append_query(param.key, utility::conversions::to_string_t(percentEncode(param.value)), false);

    //set request path
    req.set_request_uri(builder.to_string());

    //build the authorization header
    auto authorization_header = "OAuth " + percentEncode(U("oauth_consumer_key")) + "=\"" + percentEncode(OAUTH_CONSUMER_KEY) + "\", "
        + percentEncode(U("oauth_nonce")) + "=\"" + percentEncode(OAUTH_NONCE) + "\", "
        + percentEncode(U("oauth_signature")) + "=\"" + percentEncode(OAUTH_SIGNATURE) + "\", "
        + percentEncode(U("oauth_signature_method")) + "=\"" + percentEncode(OAUTH_SIGNATURE_METHOD) + "\", "
        + percentEncode(U("oauth_timestamp")) + "=\"" + percentEncode(OAUTH_TIMESTAMP) + "\", "
        + percentEncode(U("oauth_token")) + "=\"" + percentEncode(OAUTH_TOKEN) + "\", "
        + percentEncode(U("oauth_version")) + "=\"" + percentEncode(OAUTH_VERSION) + "\"";

    //add adders to the request
    req.headers().add(U("Accept"), U("/*/"));
    req.headers().add(U("Connection"), U("close"));
    req.headers().add(U("Authorization"), utility::conversions::to_string_t(authorization_header));
    
    //string to hold the response
    std::string response;
    try
    {
        // Send https request.
        pplx::task<http_response> my_request = my_client.request(req);
        my_request.then([](http_response res)
        {
            auto stat = res.status_code();
            if (stat != 200)  //200 is good status code. Everything else indicates an unusable server response
            {
                throw Apollo::Bot::BadStatusException("Bad Twitter status code", std::to_string(stat));
            }

            return res.extract_vector();
        }).then([&response](pplx::task<std::vector<unsigned char>> vector_task)
        {
            auto v = vector_task.get();
            response.assign(v.begin(), v.end());
        }).wait(); // Wait for task group to complete.
    }
    catch (const Apollo::Bot::BadStatusException& e)
    {
        throw;
    }
    catch (const std::exception& e)
    {
        throw;
    }

    return response;
}

std::vector<Apollo::Comment> Apollo::Bot::Twitter::parseJSON(const rapidjson::Document & document)
{
    std::vector<Comment> comments;

    auto temp_highest_seen = this->highest_timestamp_seen_;
    for (size_t i = 0; i < document["statuses"].Size(); ++i)
    {
        auto& tweet = document["statuses"][i];
        auto id_str = tweet["id_str"].GetString();
        std::string time = tweet["created_at"].GetString();
        comments.push_back(Comment(tweet["full_text"].GetString(), id_str));

        if (this->compareBigNumbers(id_str, temp_highest_seen))
            temp_highest_seen = id_str;
    }
    this->highest_timestamp_seen_ = temp_highest_seen;

    //update the greatest id seen in the target.
    for (auto& param : this->target_.request_parameters)
    {
        if (param.key == U("since_id"))
        {
            param.value = utility::conversions::to_string_t(highest_timestamp_seen_);
            break;
        }
    }
    return comments;
}

std::vector<Apollo::Comment> Apollo::Bot::Twitter::cleanComments(std::vector<Comment>& comments)
{
    for (auto& comment : comments)
    {
        this->trim(comment.content);
    }
    return comments;
}

utility::string_t Apollo::Bot::Twitter::generateNonce(uint64_t utc)
{
    std::vector<unsigned char> seed;
    for (int i = 0; i < 32; ++i)
        seed.push_back(utc % (31 * (i % 7 + 1)));   //generates 32 bytes of pseudo random data
    return this->stripBase64(utility::conversions::to_base64(seed));
}

void Apollo::Bot::Twitter::checkSig(const utility::string_t & sig)
{
    //this is function is just for unit testing to check that the oauth_signature is properly generated
    return;
}

Apollo::Bot::Twitter::Twitter()
{
    std::ifstream file(RESOURCE_FILE_);

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
        this->highest_timestamp_seen_ = doc["highest_timestamp_seen"].GetString();
    }
    else //create the JSON config file
    {
        this->consumer_key_ = "";
        this->consumer_secret_ = "";
        this->oauth_access_token_key_ = "";
        this->oauth_access_token_secret_ = "";
        this->highest_timestamp_seen_ = "0";
        this->saveSettings();
        throw Apollo::Bot::BadResourceException();
    }
}

Apollo::Bot::Twitter::~Twitter()
{
}

void Apollo::Bot::Twitter::setSearchQuery(const std::string & query)
{
    if (query.size() > 500)
        throw Apollo::Bot::BadTargetException("Bad Twitter target. Query must be no more than 500 characters");
    ScraperTarget target(BASE_URL_, U("/1.1/search/tweets.json"), web::http::methods::GET);
    target.request_parameters.push_back(RequestParameter(U("count"), this->MAX_SEARCH_COUNT_));
    target.request_parameters.push_back(RequestParameter(U("tweet_mode"), U("extended")));
    target.request_parameters.push_back(RequestParameter(U("q"), utility::conversions::to_string_t(query)));
    target.request_parameters.push_back(RequestParameter(U("since_id"), utility::conversions::to_string_t(this->highest_timestamp_seen_))); //since last utc param
    target.request_parameters.push_back(RequestParameter(U("result_type"), U("recent")));
    this->target_ = target;
}
