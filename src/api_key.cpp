//File: api_key.c
//Written by ANDREW LAUX
#include "api_key.h"

//Sets the API key.
void ApiKey::setKey(string_t key) {
	this->key = key;

}

//Returns the API key.
string_t ApiKey::getKey() {
	return this->key;
	
}
