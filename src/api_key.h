//FILE: api_key.h
//Written by ANDREW LAUX

#ifndef api_key_h
#define api_key_h
#include<string>

//Class: ApiKey
class ApiKey {
private:
	std::string key;
public:
	ApiKey() :key("") {}
	void setKey(std::string key);
	void clear() { this->key = std::string(""); }
	bool isComplete() { return !key.empty(); }
	std::string getKey();
	std::string getHash();
};

#endif // !api_key_h

