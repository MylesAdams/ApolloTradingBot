#include "Twitter.h"

utility::string_t Apollo::Bot::Twitter::percentEncode(const utility::string_t& s)
{
    std::string str = utility::conversions::utf16_to_utf8(s);
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
    std::cout << encoded_str;
    utf16string t = U("asdf");
    return t;
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

std::stringstream Apollo::Bot::Twitter::requestResponse(const std::string & resource_url, const std::string& request_path)
{
    // Namespace.
    using namespace utility;                    // Common utilities like string conversions.
    using namespace web;                        // Common features like URIs.
    using namespace web::http;                  // Common HTTP functionality.
    using namespace web::http::client;          // HTTP client features.
    using namespace concurrency::streams;		// Asynchronous streams.
    std::stringstream response;
    
    // Create client.
    http_client my_client(utility::conversions::to_string_t(resource_url));

    // Declare request.
    http_request req(methods::GET);

    // Get time since UNIX epoch
    uint64_t utc = datetime::utc_timestamp();
    
    utf8string method = "GET";
    string_t content_type = U("application/json");
    //encode===========
    string_t oauth_consumer_key = utility::conversions::to_string_t(this->consumer_key_);
    string_t oauth_nonce;
    string_t oauth_signature_method = U("HMAC-SHA1");
    string_t oauth_timestamp = utility::conversions::to_string_t(std::to_string(utc));
    string_t oauth_token = utility::conversions::to_string_t(this->oauth_access_token_key_);
    string_t oauth_version = U("1.0");
    //=============this
    std::vector<unsigned char> seed;
    for (int i = 0; i < 32; ++i)
        seed.push_back(utc % (31 * (i % 7 + 1)));   //generates 32 bytes of pseudo random data
    oauth_nonce = utility::conversions::to_base64(seed); //might need to be stripped of "non-word" characters

    string_t combined_url = percentEncode(U("oauth_consumer_key=") + oauth_consumer_key)
                            + percentEncode(U("&oauth_nonce=") + oauth_nonce)
                            + percentEncode(U("&oauth_signature_method=") + oauth_signature_method)
                            + percentEncode(U("&oauth_timestamp=") + oauth_timestamp)
                            + percentEncode(U("&oauth_token=") + oauth_token)
                            + percentEncode(U("&oauth_version=") + oauth_version);
    

    //// Decode base64 encoded key, secret.
    //string_t secret = U("+RDw7Q8V9EbHZusYzX3vfub0I80tytDs8RQPd3la8/wpKLShyf+B6113C3xxqiRy0r5c8UWiUmy+xSaATemWIg==");
    //std::vector<unsigned char> decoded_vec = utility::conversions::from_base64(secret);
    //std::cout << "Decoded secret has " << decoded_vec.size() << " bytes." << std::endl << std::endl;
    //std::cout << "Decoded secret is: ";
    //for (auto i : decoded_vec) {
    //    std::cout << i;
    //}

    // Dump prehash into an arry of bytes.
   // unsigned char* ary_prehash = new unsigned char[prehash.size()];
//   /* for (size_t i = 0; i < prehash.size(); i++) {
//        ary_prehash[i] = static_cast<unsigned char>(prehash[i]);
//    }
//*/
//    // Dump decoded vector into an array of bytes.
//    unsigned char* ary_key = new unsigned char[decoded_vec.size()];
//    for (size_t i = 0; i < decoded_vec.size(); i++) {
//        ary_key[i] = static_cast<unsigned char>(decoded_vec[i]);
    //}

    // Generate signature using sha256 encryption on prehash with key ary_key.
    //unsigned char encrypted[EVP_MAX_MD_SIZE];
    //unsigned int encrypted_length;
    //HMAC(EVP_sha256(), ary_key, decoded_vec.size(), ary_prehash, prehash.size(), encrypted, &encrypted_length);

    //// Add ecrypteded bytes to a vector so that it can be encoded.
    //std::vector<unsigned char> signature;
    //for (size_t i = 0; i < encrypted_length; i++) {
    //    signature.push_back(encrypted[i]);
    //}
    //for (auto i : signature) {
    //    std::cout << i;
    //}
    //std::cout << std::endl;

    //// Encode signature in base64.
    //string_t sign = conversions::to_base64(signature);

    //// Delete allocated memory.
    //delete[]ary_key;
    //delete[]ary_prehash;

    //// Add headers to http request.
    //req.headers().set_content_type(content_type);								// Sets content type to application/json.
    //req.set_request_uri(request_path);

    //my_client.request(req).then([&response](http_response res) {
    //    printf("received response status code:%u\n", res.status_code());
    //    pplx::task<std::vector<unsigned char>> task = res.extract_vector();
    //    std::vector<unsigned char> body_vec = task.get();

    //    for (auto& i : body_vec) {
    //        std::cout << i;
    //    }
    //    response << res.extract_string().get().c_str();
    //});

    return response;
}

std::vector<Apollo::Comment> Apollo::Bot::Twitter::parseJSON(const rapidjson::Document & document)
{
    return std::vector<Comment>();
}

std::vector<Apollo::Comment> Apollo::Bot::Twitter::cleanComments(std::vector<Comment>& comments)
{
    return std::vector<Apollo::Comment>();
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
        this->complete_urls_.push_back("https://api.twitter.com");
        this->request_paths_.push_back("/1.1/statuses/user_timeline.json");
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
