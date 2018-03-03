//File: gdax_accnt.cpp
//Written by ANDREW LAUX

#include"gdax_accnt.h"

// -------------------
// Default constructor
// -------------------
Apollo::Exchanges::GdaxAccnt::GdaxAccnt() :
    ExchangeAccnt(U("Gdax"), U("https://api-public.sandbox.gdax.com")) {
}

// -----------------------------
// Public method: hasCredentials
// -----------------------------
bool Apollo::Exchanges::GdaxAccnt::hasCredentials() {

    // Exchange is considered to have credentials if the following are all not empty.
    return (!key.empty() && !passphrase.empty() && !secret.empty());
}

// ---------------------
// Public method: update
// ---------------------
void Apollo::Exchanges::GdaxAccnt::update() {

    // If function was invoked without all credentials, throw exception.
    if (!this->hasCredentials()) throw std::invalid_argument("Cannot connect with missing credentials.");

    // Temp json object and temp vector to hold coins.
    web::json::value temp_json;
    std::vector<Apollo::Exchanges::Coin> temp_coins;
    Apollo::Exchanges::Coin temp_coin;

    // Prepare pre-hash string.
    utility::string_t request = U("/accounts");
    utility::string_t method = U("GET");
    utility::string_t timestamp = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp()));
    utility::string_t prehash = timestamp + method + request;

    // Dump prehash into byte vector.
    unsigned char* prehash_ary = new unsigned char[prehash.size()];
    for (size_t i = 0; i < prehash.size(); i++) {
        prehash_ary[i] = static_cast<unsigned char>(prehash[i]);
    }

    // Decode base64 secret.
    std::vector<unsigned char> decoded_secret_vec = utility::conversions::from_base64(this->secret);

    // Dump decoded_secret into byte arry.
    unsigned char* decoded_secret_ary = new unsigned char[decoded_secret_vec.size()];
    for (size_t i = 0; i < decoded_secret_vec.size(); i++) {
        decoded_secret_ary[i] = static_cast<unsigned char>(decoded_secret_vec[i]);
    }

    // Generate signature using sha256 encryption on prehash with key ary_key.
    unsigned char encrypted[EVP_MAX_MD_SIZE];
    unsigned int encrypted_length;
    HMAC(EVP_sha256(), decoded_secret_ary, decoded_secret_vec.size(), prehash_ary, prehash.size(), encrypted, &encrypted_length);

    // Add ecypred bytes to a vector so that it can be encoded.
    std::vector<unsigned char> signature;
    for (size_t i = 0; i < encrypted_length; i++) {
        signature.push_back(encrypted[i]);
    }

    // Encode signature in base64.
    utility::string_t coded_signature = utility::conversions::to_base64(signature);

    // Delete allocated memory.
    delete[]decoded_secret_ary;
    delete[]prehash_ary;

    // Crate http_request.
    web::http::http_request req(web::http::methods::GET);

    // Build http request object.
    web::uri_builder builder(U("/accounts"));
    req.headers().set_content_type(U("application/json"));            // Sets content type to application/json.
    req.set_request_uri(builder.to_string());                         // URI request path.
    req.headers().add(U("CB-ACCESS-KEY"), this->key);                 // Key header.
    req.headers().add(U("CB-ACCESS-SIGN"), coded_signature);          // Sign header.
    req.headers().add(U("CB-ACCESS-TIMESTAMP"), timestamp);           // Timestamp header.
    req.headers().add(U("CB-ACCESS-PASSPHRASE"), this->passphrase);   // Passphrase header.

                                                                      // Create client.
    web::http::client::http_client gdax_client(this->url);

    // Check connectivity.
    pplx::task<web::http::http_response> gdax_call = gdax_client.request(req);
    gdax_call.then([](web::http::http_response response) {
        if (response.status_code() != web::http::status_codes::OK) {
            std::string msg = "Status code from Gdax server was not OK: ";
            std::string status = std::to_string(response.status_code());
            throw Apollo::Bot::BadStatusException(msg, status);
        }

        //Extract json object.
        return response.extract_json();
   
    // Get extracted json.
    }).then([&temp_json](web::json::value extracted) {
    
        //Assign json.
        temp_json = extracted;

    // Get remainign I/O.
    }).wait();

    // Process coins from json.
    for (size_t i = 0; i < temp_json.size(); i++) {
        auto currency_val = temp_json[i][U("currency")];
        utility::string_t currency = currency_val.as_string();
        auto amount_val = temp_json[i][U("balance")];
        double temp_amount = std::stod(amount_val.as_string());
        if (currency != U("USD") && currency != U("EUR") && currency != U("GBP")) {
            temp_coin.amount = temp_amount;
            temp_coin.coin_id = currency;
            temp_coins.push_back(temp_coin);
        }
    }

    // Set exchange's coins.
    this->coins_vec = temp_coins;

}

// -----------------
// Function: connect
// -----------------
void Apollo::Exchanges::GdaxAccnt::connect() {

    // If function was invoked without all credentials, throw exception.
    if (!this->hasCredentials()) throw std::invalid_argument("Cannot connect with missing credentials.");

    // Temp connectivity boolean.
    bool connectivity = false;

    // Prepare pre-hash string.
    utility::string_t request = U("/accounts");
    utility::string_t method = U("GET");
    utility::string_t timestamp = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp()));
    utility::string_t prehash = timestamp + method + request;

    // Dump prehash into byte vector.
    unsigned char* prehash_ary = new unsigned char[prehash.size()];
    for (size_t i = 0; i < prehash.size(); i++) {
        prehash_ary[i] = static_cast<unsigned char>(prehash[i]);   
    }

    // Decode base64 secret.
    std::vector<unsigned char> decoded_secret_vec = utility::conversions::from_base64(this->secret);

    // Dump decoded_secret into byte arry.
    unsigned char* decoded_secret_ary = new unsigned char[decoded_secret_vec.size()];
    for (size_t i = 0; i < decoded_secret_vec.size(); i++) {
        decoded_secret_ary[i] = static_cast<unsigned char>(decoded_secret_vec[i]);
    }

    // Generate signature using sha256 encryption on prehash with key ary_key.
    unsigned char encrypted[EVP_MAX_MD_SIZE];
    unsigned int encrypted_length;
    HMAC(EVP_sha256(), decoded_secret_ary, decoded_secret_vec.size(), prehash_ary, prehash.size(), encrypted, &encrypted_length);

    // Add ecypred bytes to a vector so that it can be encoded.
    std::vector<unsigned char> signature;
    for (size_t i = 0; i < encrypted_length; i++) {
        signature.push_back(encrypted[i]);
    }

    // Encode signature in base64.
    utility::string_t coded_signature = utility::conversions::to_base64(signature);

    // Delete allocated memory.
    delete[]decoded_secret_ary;
    delete[]prehash_ary;

    // Crate http_request.
    web::http::http_request req(web::http::methods::GET);

    // Build http request object.
    web::uri_builder builder(U("/accounts"));
    req.headers().set_content_type(U("application/json"));            // Sets content type to application/json.
    req.set_request_uri(builder.to_string());                         // URI request path.
    req.headers().add(U("CB-ACCESS-KEY"), this->key);                 // Key header.
    req.headers().add(U("CB-ACCESS-SIGN"), coded_signature);          // Sign header.
    req.headers().add(U("CB-ACCESS-TIMESTAMP"), timestamp);           // Timestamp header.
    req.headers().add(U("CB-ACCESS-PASSPHRASE"), this->passphrase);   // Passphrase header.
  
    // Create client.
    web::http::client::http_client gdax_client(this->url);

    // Check connectivity.
    pplx::task<web::http::http_response> gdax_call = gdax_client.request(req);
    gdax_call.then([&connectivity](web::http::http_response response) {
        if (response.status_code() == web::http::status_codes::OK) connectivity = true;
        else connectivity = false;
        return response.extract_string();

    // Get remainign I/O.
    }).wait();

    // Modify state boolean in base class.
    this->connected = connectivity;
}

// ---------------------------------------------------------
// Public method: setCredentials - Correct overload for Gdax.
// ---------------------------------------------------------
void Apollo::Exchanges::GdaxAccnt::setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase) {
    this->key = key;
    this->secret = secret;
    this->passphrase = passphrase;
}

// -------------------------------
// Public method: clearCredentials
// -------------------------------
void Apollo::Exchanges::GdaxAccnt::clearCredentials() {
    setCredentials(U(""), U(""), U(""));
}


