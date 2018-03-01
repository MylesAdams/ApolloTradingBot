//File: gdax_accnt.cpp
//Written by ANDREW LAUX

#include"gdax_accnt.h"

// Default constructor 
GdaxAccnt::GdaxAccnt() :
    ExchangeAccnt(U("Gdax"), U("https://api-public.sandbox.gdax.com")) {
}

// Fucntion: hasCredentials
bool GdaxAccnt::hasCredentials() {

    // Exchange is considered access if credentials are not empty.
    return (!key.empty() && !passphrase.empty() && !secret.empty());
}

// Function: connect 
void GdaxAccnt::connect() {

    // If function was invoked without all credentials, throw exception.
    if (!this->hasCredentials()) throw std::invalid_argument("Cannot connect with missing credentials.");

    // Temp connectivity boolean.
    bool connectivity = false;

    // Prepare pre-hash string.
    string_t request = U("/accounts");
    string_t method = U("GET");
    string_t timestamp = std::to_wstring(utility::datetime::utc_timestamp());
    string_t prehash = timestamp + method + request;

    // Dump prehash into byte vector.
    unsigned char* prehash_ary = new unsigned char[prehash.size()];
    for (size_t i = 0; i < prehash.size(); i++) {
        prehash_ary[i] = static_cast<unsigned char>(prehash[i]);   
    }

    // Decode base64 secret.
    std::vector<unsigned char> decoded_secret_vec = utility::conversions::from_base64(this->secret);

    // Dump decoded_secret into byte arry.
    unsigned char* decoded_secret_ary = new unsigned char[decoded_secret_vec.size()];
    for (size_t i = 0; i < prehash.size(); i++) {
        decoded_secret_ary[i] = decoded_secret_vec[i];
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
    string_t coded_signature = conversions::to_base64(signature);

    // Delete allocated memory.
    delete[]decoded_secret_ary;
    delete[]prehash_ary;

    // Crate http_request.
    web::http::http_request req(web::http::methods::GET);

    // Add headers to http request.
    req.headers().set_content_type(U("application/json"));            // Sets content type to application/json.
    req.set_request_uri(request);                                     // URI request path.
    req.headers().add(U("CB-ACCESS-KEY"), this->key);                 // Key header.
    req.headers().add(U("CB-ACCESS-SIGN"), coded_signature);          // Sign header.
    req.headers().add(U("CB-ACCESS-TIMESTAMP"), timestamp);           // Timestamp header.
    req.headers().add(U("CB-ACCESS-PASSPHRASE"), this->passphrase);   // Passphrase header.

    // Create client.
    web::http::client::http_client gdax_client(this->url);

    // Check connectivity.
    pplx::task<web::http::http_response> gdax_call = gdax_client.request(req);
    gdax_call.then([&connectivity](web::http::http_response respose) {
        if (respose.status_code() == web::http::status_codes::OK) connectivity = true;
        else connectivity = false;
    }).wait();

    // Modify state boolean in base class.
    this->connected = connectivity;
}

// Public method: setCredentials - Correct overload for Gdax.
void GdaxAccnt::setCredentials(string_t key, string_t secret, string_t passphrase) {

    // Set credentials
    this->key = key;
    this->secret = secret;
    this->passphrase = passphrase;
}


