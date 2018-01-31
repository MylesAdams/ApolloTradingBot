//File: api_key.c
//Written by ANDREW LAUX
#include "api_key.h"

//Sets the API key.
void ApiKey::setKey(std::string key) {
	this->key = key;
	//Fucntion should implement hashing.
}

//Returns the API key.
std::string ApiKey::getKey() {
	return std::string();
	//Function should unhash key.
}

//Returns the hashed API key. For storage.
std::string ApiKey::getHash() {
	return std::string();
}
