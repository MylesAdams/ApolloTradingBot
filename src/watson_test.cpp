// File: watson_test.cpp
// Written by ANDREW LAUX

#include <iostream>
#include <cpprest\http_client.h>
#include <cpprest\json.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

// Test driver.
int main() {


	// Sentiment to analyze.
	string_t comment = U("We don't serve droids here.\nI love you.\n This is gonna be good!");

	// Credentials.
	credentials cred(U("c032fda0-5c02-490d-8e00-ab00de2e5f40"), U("AfgP2LQIDCgB"));

	// Create client config. 
	http_client_config config;
	config.set_credentials(cred);

	// Create client.
	http_client watson_client(U("https://gateway.watsonplatform.net/tone-analyzer/api/"), config);

	// Build uri.
	uri_builder builder(U("/v3/tone"));
	builder.append_query(U("version"), U("2017-09-21"));
	builder.append_query(U("sentences"), U("false"));

	// Create request.
	http_request req(methods::POST);
	req.set_request_uri(builder.to_string());
	req.set_body(comment);

	//String to hold serialized json.
	string_t json_body;

	// Attempt request.
	pplx::task<http_response> watson_call = watson_client.request(req);
	watson_call

	// Hookup response continuation
	.then([](http_response response) {

		// Report error.
		std::cout << response.status_code() << std::endl;

		// Extract body into vector.
		return response.extract_string();

	})
	
	// Hookup continuation to process body.
	.then([&json_body](pplx::task<string_t> string_task) {

		//Process extracted json.
		json_body = string_task.get();

	}).wait(); // Collect outstanding tasks.

	// Exit program.
	std::cin.get();
	return 0;
}