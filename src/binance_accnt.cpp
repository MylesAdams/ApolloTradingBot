//File: binance_accnt.cpp
//Written by ANDREW LAUX

#include"binance_accnt.h"

//--------------------
// Default Constructor
//--------------------
Apollo::Exchanges::BinanceAccnt::BinanceAccnt() :
    ExchangeAccnt(U("Binance"), U("https://api.binance.com")) {
}

//------------------------------
// Public method: hasCredentials
//------------------------------
bool Apollo::Exchanges::BinanceAccnt::hasCredentials() {
    return (!this->key.empty() && !this->secret.empty());
}

//----------------------
// Public method: update
//----------------------
void Apollo::Exchanges::BinanceAccnt::update() {

    // If function was invoked without all credentials, throw exception.
    if (!this->hasCredentials()) throw std::invalid_argument("Cannot connect with missing credentials.");

    //TODO: complete
}

//-----------------------
// Public method: connect
//-----------------------
void Apollo::Exchanges::BinanceAccnt::connect() {
    //TODO: complete
}

//--------------------------------
// Public method: clearCredentials
//--------------------------------
void Apollo::Exchanges::BinanceAccnt::clearCredentials() {
    setCredentials(U(""), U(""));
}

//------------------------------
// Public method: setCredentials
//------------------------------
void Apollo::Exchanges::BinanceAccnt::setCredentials(utility::string_t key, utility::string_t secret) {
    this->key = key;
    this->secret = secret;
}
