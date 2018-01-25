//file: log_in.h
//written by ANDREW LAUX.

#ifndef log_in_h
#define log_in_h
#include<string>

//Class: LogIn
class LogIn {
private:
	std::string user;
	std::string pass;
	std::string key;
public:
	LogIn() :user(""), pass(""), key("") {};
	void setLogIn(std::string user, std::string pass, std::string key);
	void clear();
	bool isComplete();
	std::string getUser();
	std::string getPass();
	std::string getKey();
};

#endif // !log_in.h
