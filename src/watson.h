// File: watson.h
// Written by ANDREW LAUX

#ifndef watson_h
#define watson_h
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>
#include <fstream>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features

// Class: Watson
class Watson {
private:
	http_client_config config;		// Passed to client constructor. Holds username and pass.
	credentials creds;				// Holds username and pass. Passed to config.
	http_request req;				// Http request object. Has method and uri.
	uri_builder builder;			// URI object.
	datetime date;					// Use date to get UTC timestamp.
public:
	Watson(string_t user, string_t pass);
	string_t toneToString(const string_t &tone_input);								// Throws exception.
	void toneRatingToFile(const string_t &tone_input, const string_t &filename);	// Throws exception.
private:
	Watson();
	Watson(Watson &copy) = delete;								// Forbid copy constructor.
	Watson& operator=(Watson &copy) = delete;					// Forbid assignment.
	int rateTone(const string_t &tone);							// Helper. Rates tones. -1, 0, or 1,	
};


#endif // !watson_h
