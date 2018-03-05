//File: kucoin_accnt.cpp
//Written by ANDREW LAUX

#include"kucoin_accnt.h"

//////////////////////
// Default constructor
//////////////////////
Apollo::Exchanges::KucoinAccnt::KucoinAccnt() :
    ExchangeAccnt(U("Kucoin"), U("https://api.kucoin.com")) {
}

/////////////////////////////////
// Public method: hasCredentials
/////////////////////////////////
bool Apollo::Exchanges::KucoinAccnt::hasCredentials() {
    return (!this->key.empty() && !this->secret.empty());
}

////////////////////////
// Public method: update
////////////////////////
void Apollo::Exchanges::KucoinAccnt::update() {
    // TODO:  complete
}

/////////////////////////
// Public method: connect
/////////////////////////
void Apollo::Exchanges::KucoinAccnt::connect() {
    
    // Generate signature
    utility::string_t endpoint = U("/v1/user/info");
    utility::string_t nonce = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp())) + U("999");
    utility::string_t prehash = endpoint + U("/") + nonce + U("/");

    // Convert prehash to vector of bytes.
    std::vector<unsigned char> prehash_vec;
    for (size_t i = 0; i < prehash.size(); i++) {
        prehash_vec.push_back(static_cast<unsigned char>(prehash[i]));
    }

    // Base64 encode prehash.
    utility::string_t encoded = utility::conversions::to_base64(prehash_vec);

    // Convert encoded prehash to  array of bytes.
    unsigned char* encoded_ary = new unsigned char[encoded.size()];
    for (size_t i = 0; i < encoded.size(); i++) {
        encoded_ary[i] = static_cast<unsigned char>(encoded[i]);
    }

    // Convert secret to array of bytes.
    unsigned char* secret_ary = new unsigned char[this->secret.size()];
    for (size_t i = 0; i < this->secret.size(); i++) {
        secret_ary[i] = static_cast<unsigned char>(secret[i]);
    }

    // Sha256 encryption.
    unsigned char encrypted[EVP_MAX_MD_SIZE];
    unsigned int encrypted_length;
    HMAC(EVP_sha256(), secret_ary, this->secret.size(), encoded_ary, encoded.size(), encrypted, &encrypted_length);
 
    // Hex encode encrypted output.
    std::stringstream ss;
    for (size_t i = 0; i < encrypted_length; i++) {
        ss << std::hex << (int)encrypted[i];
    }
    utility::string_t signature = utility::conversions::to_string_t(ss.str());

    // Create request.
    web::http::http_request req(web::http::methods::GET);
    req.set_request_uri(endpoint);
    req.headers().set_content_type(U("application/json"));
    req.headers().add(U("KC-API-KEY"), this->key);
    req.headers().add(U("KC-API-NONCE"), nonce);
    req.headers().add(U("KC-API-SIGNATURE"), signature);

    //Create client.
    web::http::client::http_client myclient(this->url);

    // Make call to server.
    pplx::task<web::http::http_response> kucoin_call = myclient.request(req);
    web::http::http_response response = kucoin_call.get();

    // Check connectivity.
    if (response.status_code() == web::http::status_codes::OK) {
        this->connected = true;     
    }
    else this->connected = false;

    // Extract string from json.
    pplx::task<utility::string_t> extract_task = response.extract_string();
    utility::string_t extracted = extract_task.get();

    ucout << extracted;
    ucout << nonce;
}

///////////////////////////////////
// Publice method: clearCredentials
///////////////////////////////////
void Apollo::Exchanges::KucoinAccnt::clearCredentials() {
    setCredentials(U(""), U(""));
}

////////////////////////////////
// Public method: setCredentials
////////////////////////////////
void Apollo::Exchanges::KucoinAccnt::setCredentials(utility::string_t key, utility::string_t secret) {
    this->key = key;
    this->secret = secret;
}
