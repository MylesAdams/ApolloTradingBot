//File: kucoin_accnt.cpp
//Written by ANDREW LAUX

#include"kucoin_accnt.h"

//////////////////////
// Default constructor
//////////////////////
Apollo::Exchanges::KucoinAccnt::KucoinAccnt() {
    // TODO: complete
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
    // TODO: complete
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
