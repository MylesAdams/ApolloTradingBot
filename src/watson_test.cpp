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
	string_t comment = U("We don't serve droids here.");

	// Create client.
	http_client watson_client(U("https://gateway.watsonplatform.net/tone-analyzer/api/v3/"));

	// Create request.
	http_request req(methods::POST);
	req.headers().set_content_type(U("text/plain"));
	req.set_body(comment);

	// Attempt request.
	pplx::task<http_response> watson_call = watson_client.request(req);
	watson_call

	// Hookup response continuation
	.then([](http_response response) {

		// Report error.
		std::cout << response.status_code() << std::endl;

		// Extract body into vector.
		return response.extract_vector();

	})
	
	// Hookup continuation to process body.
	.then([](pplx::task<std::vector<unsigned char>> extracted_vec) {

		// Get the vector.
		std::vector<unsigned char> body = extracted_vec.get();

		//Print vector by range based for loop.
		for (auto i : body) {
			std::cout << i;
		}

	}).wait(); // Collect outstanding tasks.

	// Exit program.
	std::cin.get();
	return 0;
}