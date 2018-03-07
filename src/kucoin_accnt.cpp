//File: kucoin_accnt.cpp
//Written by ANDREW LAUX

#include"kucoin_accnt.h"

//////////////////////
// Default constructor
//////////////////////
Apollo::Exchanges::KucoinAccnt::KucoinAccnt() :
    ExchangeAccnt(U("Kucoin"), U("https://api.kucoin.com")),
    num_pages(0) {
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
    
    // Generate signature
    utility::string_t endpoint = U("/v1/account/balances");
    utility::string_t querystring = U("limit=20&page=1"); 
    utility::string_t nonce = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp())) + U("000");
    utility::string_t prehash = endpoint + U("/") + nonce + U("/") + querystring;

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
    utility::string_t signature;
    int integer;
    wchar_t hextable[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    for (size_t i = 0; i < encrypted_length; i++) {
        integer = encrypted[i];
        signature.push_back(hextable[0x0000000f & integer >> 4]);
        signature.push_back(hextable[0x0000000f & integer]);
    }

    // Create request.
    web::http::http_request req(web::http::methods::GET);
    web::uri_builder builder(endpoint);
    builder.append_query(U("limit=20"));
    builder.append_query(U("page=1"));
    req.set_request_uri(builder.to_string());
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

        // Extract string from json.
        pplx::task<web::json::value> extract_task = response.extract_json();
        web::json::value extracted = extract_task.get();
        utility::string_t extrct_str = extracted.serialize();

        auto total_val = extracted[U("data")][U("pageNos")];
        this->num_pages = total_val.as_integer();

        // Make a request for each page.
        int page;
        size_t entries_on_page;
        for (int p = 0; p < this->num_pages; p++) {
            page = p + 1;

            // Create request for account balance by page.
            querystring = U("limit=20&page=") + page;
            nonce = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp()));
            nonce = nonce + U("00") + utility::conversions::to_string_t(std::to_string(page));
            prehash = endpoint + U("/") + nonce + U("/") + querystring;
            std::vector<unsigned char> prehash_vec_temp;
            for (size_t i = 0; i < prehash.size(); i++) { prehash_vec_temp.push_back(static_cast<unsigned char>(prehash[i])); }
            encoded = utility::conversions::to_base64(prehash_vec_temp);
            unsigned char* encoded_ary_temp = new unsigned char[encoded.size()];
            for (size_t i = 0; i < encoded.size(); i++) { encoded_ary_temp[i] = static_cast<unsigned char>(encoded[i]); }
            
            unsigned char encrypted_temp[EVP_MAX_MD_SIZE];
            encrypted_length = 0;
            HMAC(EVP_sha256(), secret_ary, this->secret.size(), encoded_ary_temp, encoded.size(), encrypted_temp, &encrypted_length);
            utility::string_t signature_temp;
            for (size_t i = 0; i < encrypted_length; i++) {
                integer = encrypted_temp[i];
                signature_temp.push_back(hextable[0x0000000f & integer >> 4]);
                signature_temp.push_back(hextable[0x0000000f & integer]);
            }
            web::http::http_request req_temp(web::http::methods::GET);
            web::uri_builder builder_temp(endpoint);
            builder_temp.append_query(U("limit=20"));
            builder_temp.append_query(U("page=")+ page);
            req_temp.set_request_uri(builder_temp.to_string());
            req_temp.headers().set_content_type(U("application/json"));
            req_temp.headers().add(U("KC-API-KEY"), this->key);
            req_temp.headers().add(U("KC-API-NONCE"), nonce);
            req_temp.headers().add(U("KC-API-SIGNATURE"), signature_temp);

            // Make call to server.
            pplx::task<web::http::http_response> call_task = myclient.request(req_temp);
            web::http::http_response response_temp = call_task.get();

            // Check response status.
            if (response_temp.status_code() == web::http::status_codes::OK) {
                pplx::task<web::json::value> temp_extract_task = response_temp.extract_json();
                web::json::value temp_json = temp_extract_task.get();
                entries_on_page = temp_json[U("data")][U("datas")].size();
            }

        }
    }

    // Bad server response.
    else this->connected = false;
    
   
}

/////////////////////////
// Public method: connect
/////////////////////////
void Apollo::Exchanges::KucoinAccnt::connect() {
    
    // Generate signature
    utility::string_t endpoint = U("/v1/user/info");
    utility::string_t nonce = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp())) + U("000");
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
    utility::string_t signature;
    int integer;
    wchar_t hextable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    for (size_t i = 0; i < encrypted_length; i++) {
        integer = encrypted[i];
        signature.push_back(hextable[0x0000000f & integer >> 4]);
        signature.push_back(hextable[0x0000000f & integer]);
    }

    // Create request.
    web::http::http_request req(web::http::methods::GET);
    req.set_request_uri(this->url + endpoint);
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
