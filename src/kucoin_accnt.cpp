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
    
    // Generate signature.
    utility::string_t endpoint = U("/v1/account/balances");
    utility::string_t querystring = U("limit=20&page=1"); 
    utility::string_t nonce = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp())) + U("100");
    utility::string_t prehash = endpoint + U("/") + nonce + U("/") + querystring;

    // Get call response.
    web::http::http_response response = getResponse(endpoint, nonce, querystring);

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
        std::vector<Apollo::Exchanges::Coin> new_vec;
        for (int p = 0; p < this->num_pages; p++) {
            page = p + 1;
            
            // New querystring and nonce.
            utility::string_t new_query = U("limit=20&page=") + utility::conversions::to_string_t(std::to_string(page));
            utility::string_t new_nonce = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp()));
            new_nonce = new_nonce + U("00") + utility::conversions::to_string_t(std::to_string(page));

            // Get response.
            web::http::http_response new_response = getResponse(endpoint, new_nonce, new_query);
            
            // If OK do processing of response.
            if (new_response.status_code() == web::http::status_codes::OK) {
                pplx::task<web::json::value> extract_json = new_response.extract_json();
                web::json::value new_json = extract_json.get();

                // Process json.
                entries_on_page = new_json[U("data")][U("datas")].size();
                for (size_t i = 0; i < entries_on_page; i++) {
                    auto id_val = new_json[U("data")][U("datas")][i][U("coinType")];
                    auto balance_val = new_json[U("data")][U("datas")][i][U("balanceStr")];
                    Apollo::Exchanges::Coin new_coin;
                    new_coin.coin_id = id_val.as_string();
                    new_coin.amount = std::stod(balance_val.as_string());
                    new_vec.push_back(new_coin);
                }

                // Set exchange's coin vector.
                this->coins_vec = new_vec;
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

    // Get call response.
    web::http::http_response response = getResponse(endpoint, nonce);

    // Check connectivity.
    if (response.status_code() == web::http::status_codes::OK) {
        this->connected = true;     
    }
    else this->connected = false;
}

/////////////////////
// Public method: buy
/////////////////////
bool Apollo::Exchanges::KucoinAccnt::buy(utility::string_t coin_id, double amount) {
    return false;
}

///////////////////////////////////
// Publice method: clearCredentials
///////////////////////////////////
void Apollo::Exchanges::KucoinAccnt::clearCredentials() {
    setCredentials(U(""), U(""));
}

////////////////////////////
// Public method: buyAtLimit
////////////////////////////
bool Apollo::Exchanges::KucoinAccnt::buyAtLimit(utility::string_t coin_id, utility::string_t amount, utility::string_t price) {
    
    // Trasnaction success flag.
    bool transaction_success = false;

    // Generate signature.
    utility::string_t method = U("POST");
    utility::string_t endpoint = U("/v1/order");
    utility::string_t querystring = U("amount=") + amount + U("&price=") + price + U("&symbol=") + coin_id + U("-BTC");
    querystring = querystring + U("&type=BUY");
    utility::string_t nonce = utility::conversions::to_string_t(std::to_string(utility::datetime::utc_timestamp())) + U("000");
    utility::string_t prehash = endpoint + U("/") + nonce + U("/") + querystring;

    // Check response.
    web::http::http_response response = getResponse(endpoint, nonce, querystring, method);
    if (response.status_code() == web::http::status_codes::OK) {
        
        // Extract json form response.
        pplx::task<web::json::value> extract_task = response.extract_json();
        web::json::value json = extract_task.get();
        utility::string_t msg = json.serialize();
        ucout << msg << U("\n");

        auto success_val = json[U("success")];
        transaction_success = success_val.as_bool();
    }

    return transaction_success;
}


////////////////////////////////
// Public method: setCredentials
////////////////////////////////
void Apollo::Exchanges::KucoinAccnt::setCredentials(utility::string_t key, utility::string_t secret) {
    this->key = key;
    this->secret = secret;
}

////////////////////////////////////////
// Private helper function: getResponse
////////////////////////////////////////
web::http::http_response Apollo::Exchanges::KucoinAccnt::getResponse(utility::string_t endpoint, utility::string_t nonce, utility::string_t querystring, utility::string_t method) {

    // Generate prehash string.
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
    web::http::http_request req;
    if (method == U("POST")) req = web::http::http_request(web::http::methods::POST);
    web::uri_builder builder(endpoint);
    builder.append_query(querystring);
    req.set_request_uri(builder.to_string());
    req.headers().set_content_type(U("application/json"));
    req.headers().add(U("KC-API-KEY"), this->key);
    req.headers().add(U("KC-API-NONCE"), nonce);
    req.headers().add(U("KC-API-SIGNATURE"), signature);
    utility::string_t req_str = req.to_string();


    //Create client.
    web::http::client::http_client myclient(this->url);

    // Make call to server.
    pplx::task<web::http::http_response> kucoin_call = myclient.request(req);
    web::http::http_response response = kucoin_call.get();

    return response;
}
