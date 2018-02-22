// File: watson_test.cpp
// Written by ANDREW LAUX

#include <iostream>
#include <cpprest\http_client.h>
#include <cpprest\json.h>
#include "watson.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

// Test driver.
int main() {

	// Sentiment to analyze.
	string_t comment = U("We don't serve droids here.\nI love you.\n This is gonna be good!\n Just the tip, baby. \n Obama is a nagger. \n You smell like eggs. \n I'm looking forward to killing myself. \n Panda express tasted really good today. \n My mother won't stop calling me. \n When I fart it smells like toothpaste. \n");

	// Watson obj.
	Watson watson_bot(U("c032fda0-5c02-490d-8e00-ab00de2e5f40"), U("AfgP2LQIDCgB"));

	// Check.
	watson_bot.toneToFile(comment, U("c:/users/Andrew Laux/Documents/ApolloTradingBot/resources/test.json"));
	

	// Exit program.
	std::cin.get();
	return 0;
}
