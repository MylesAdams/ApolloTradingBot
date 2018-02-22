// FILE: api_key.h
// Written by ANDREW LAUX

#ifndef api_key_h
#define api_key_h

// Includes.
#include <cpprest/http_client.h>

// Define namespace.
using namespace utility;

// Class: ApiKey
class ApiKey {
private:
	string_t key;
	string_t secret;
	string_t
public:
	ApiKey() :key(U("")) {}
	void setKey(string_t key);
	void clear() { this->key = string_t(U("")); }
	bool isComplete() { return !key.empty(); }
	string_t getKey();
};

#endif // !api_key_h

