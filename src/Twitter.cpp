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
    // Namespace.
    using namespace utility;                    // Common utilities like string conversions.
    using namespace web;                        // Common features like URIs.
    using namespace web::http;                  // Common HTTP functionality.
    using namespace web::http::client;          // HTTP client features.
    using namespace concurrency::streams;		// Asynchronous streams.
    std::stringstream response;

    // Create client.
    http_client my_client(U("https://api-public.sandbox.gdax.com"));

    // Declare request.
    http_request req(methods::GET);

    // Create datetime object for timestamp.
    datetime date;
    uint64_t utc = date.utc_timestamp();


    // Generate prehash string.								
    string_t time_stamp = utility::conversions::to_string_t(std::to_string(utc));
    string_t method = U("GET");									// String of http method, "GET" in this case.
    string_t request_path = U("/accounts");						// The request path is "/accounts".
    string_t prehash = time_stamp + method + request_path;		// Concatenate fields into string to be hashed.


                                                                // Decode base64 encoded key, secret.
    string_t secret = U("+RDw7Q8V9EbHZusYzX3vfub0I80tytDs8RQPd3la8/wpKLShyf+B6113C3xxqiRy0r5c8UWiUmy+xSaATemWIg==");
    std::vector<unsigned char> decoded_vec = utility::conversions::from_base64(secret);
    std::cout << "Decoded secret has " << decoded_vec.size() << " bytes." << std::endl << std::endl;
    std::cout << "Prehash size is " << prehash.size() << std::endl;
    std::cout << "Decoded secret is: ";
    for (auto i : decoded_vec) {
        std::cout << i;
    }
    std::cout << std::endl;

    // Dump prehash into an arry of bytes.
    unsigned char* ary_prehash = new unsigned char[prehash.size()];
    for (size_t i = 0; i < prehash.size(); i++) {
        ary_prehash[i] = static_cast<unsigned char>(prehash[i]);
    }

    // Dump decoded vector into an array of bytes.
    unsigned char* ary_key = new unsigned char[decoded_vec.size()];
    for (size_t i = 0; i < decoded_vec.size(); i++) {
        ary_key[i] = static_cast<unsigned char>(decoded_vec[i]);
    }

    // Generate signature using sha256 encryption on prehash with key ary_key.
    unsigned char encrypted[EVP_MAX_MD_SIZE];
    unsigned int encrypted_length;
    HMAC(EVP_sha256(), ary_key, decoded_vec.size(), ary_prehash, prehash.size(), encrypted, &encrypted_length);

    // Add ecypred bytes to a vector so that it can be encoded.
    std::vector<unsigned char> signature;
    for (size_t i = 0; i < encrypted_length; i++) {
        signature.push_back(encrypted[i]);
    }
    for (auto i : signature) {
        std::cout << i;
    }
    std::cout << std::endl;

    // Encode signature in base64.
    string_t sign = conversions::to_base64(signature);


    // Delete allocated memory.
    delete[]ary_key;
    delete[]ary_prehash;

    // Add headers to http request.
    req.headers().set_content_type(U("application/json"));								// Sets content type to application/json.
    req.set_request_uri(request_path);
    req.headers().add(U("CB-ACCESS-KEY"), U("c0d41169560a191c7817c11b6ba4908b"));		// Key header.
    req.headers().add(U("CB-ACCESS-SIGN"), sign);										// Sign header.
    req.headers().add(U("CB-ACCESS-TIMESTAMP"), time_stamp);							// Timestamp header.
    req.headers().add(U("CB-ACCESS-PASSPHRASE"), U("mfsacc5sm7"));						// Passphrase header.

                                                                                        //generate request task and response continuation.
    my_client.request(req).then([&response](http_response res) {
        printf("received response status code:%u\n", res.status_code());
        pplx::task<std::vector<unsigned char>> task = res.extract_vector();
        std::vector<unsigned char> body_vec = task.get();

        for (auto& i : body_vec) {
            std::cout << i;
        }
        response << res.extract_string().get().c_str();
    });

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
        this->complete_urls_.push_back("test");
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
