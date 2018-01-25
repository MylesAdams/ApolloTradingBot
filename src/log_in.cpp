//File: log_in.cpp
//Written by ANDREW LAUX

#include"log_in.h"

//Set function: setLogin
void LogIn::setLogIn(std::string user, std::string pass, std::string key) {
	this->user = user;
	this->pass = pass;
	this->key = key;
	//Needs hasing. Ok for now.
}

//Mutator function: clear
void LogIn::clear() {
	//Sets all fields to empty strings.
	this->user = "";
	this->pass = "";
	this->key = "";
}

//Status function: isComplete
bool LogIn::isComplete() {
	//LogIn object considered incomplete if any field is empty.
	return !(this->key.empty() || this->pass.empty() || this->user.empty());
}

//Access function: getUser
std::string LogIn::getUser() {
	//Should unhash user field. Ok for now.
	return this->user;
}

//Access function: getPass
std::string LogIn::getPass() {
	//Should unhash pass field. Ok for now.
	return this->pass;
}

//Access function: getKey
std::string LogIn::getKey() {
	//Returns API key.
	return this->key;
}