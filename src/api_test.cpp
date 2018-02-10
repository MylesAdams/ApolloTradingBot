// File: api_test.cpp
// Written by ANDREW LAUX

// Preprocessor commands.
#include <cpprest\http_client.h>
#include <cpprest\filestream.h>
#include <cpprest\json.h>  
#include <openssl\hmac.h>
#include <vector>
#include <iostream>


// Namespace.
using namespace utility;                    // Common utilities like string conversions.
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality.
using namespace web::http::client;          // HTTP client features.
using namespace concurrency::streams;		// Asynchronous streams.

// Begin main.
int main(int argc, char* argv[]) {

	// Create client.
	http_client my_client(U("https://api-public.sandbox.gdax.com"));

	// Declare request.
	http_request req(methods::GET);

	// Create datetime object for timestamp.
	datetime date;
	uint64_t utc = date.utc_timestamp();


	// Generate prehash string.								
	string_t time_stamp = utility::conversions::to_string_t(std::to_string(utc));
	string_t method = U("GET");									// String of http method, "GET" in this case.
	string_t request_path = U("/accounts");						// The request path is "/accounts".
	string_t prehash = time_stamp + method + request_path;		// Concatenate fields into string to be hashed.
	

	// Decode base64 encoded key, secret.
	string_t secret = U("lYslfEnPL8fODyE1fpqYCD4ojTwgMMRb/C7nfhIkWj8F3jixocVH8MmhV2jE9RHHMYt3t53U59Jsqe6mNhzAWA==");
	std::vector<unsigned char> decoded_vec = utility::conversions::from_base64(secret);
	std::cout << "Decoded secret has " << decoded_vec.size() << " bytes." << std::endl << std::endl;

	std::cout << "Prehash size is " << prehash.size() << std::endl;

	// Dump prehash into an arry of bytes.
	unsigned char* ary_prehash = new unsigned char[prehash.size()];
	for (int i = 0; i < prehash.size(); i++) {
		ary_prehash[i] = prehash[i];
	}



	// Dump decoded vector into an array of bytes.
	unsigned char* ary_key = new unsigned char[decoded_vec.size()];
	for (int i = 0; i < decoded_vec.size(); i++) {
		ary_key[i] = decoded_vec[i];
	}

	// Generate signature using sha256 encryption on prehash with key ary_key.
	unsigned char encrypted[EVP_MAX_MD_SIZE];
	unsigned int encrypted_length;
	HMAC(EVP_sha256(), ary_key, decoded_vec.size(), ary_prehash, prehash.size(), encrypted, &encrypted_length);

	// Add ecypred bytes to a vector so that it can be encoded.
	std::vector<unsigned char> signature;
	for (int i = 0; i < encrypted_length; i++) {
		signature.push_back(encrypted[i]);
	}

	// Encode signature in base64.
	string_t sign = conversions::to_base64(signature);

	
	// Delete allocated memory.
	delete[]ary_key;
	delete[]ary_prehash;

	// Add headers to http request.
	req.headers().set_content_type(U("application/json"));								// Sets content type to application/json.
	req.set_request_uri(U("GET/accounts"));
	req.headers().add(U("CB-ACCESS-KEY"), U("8da062e957587b4cbb021e92d69ea459"));		// Key header.
	req.headers().add(U("CB-ACCESS-SIGN"), sign);										// Sign header.
	req.headers().add(U("CB-ACCESS-TIMESTAMP"), time_stamp);							// Timestamp header.
	req.headers().add(U("CB-ACCESS-PASSPHRASE"), U("myc2rfd2f9"));						// Passphrase header.

	//generate request task and response continuation.
	my_client.request(req).then([](http_response response) {
		printf("received response status code:%u\n", response.status_code());
		pplx::task<std::vector<unsigned char>> task = response.extract_vector();
		std::vector<unsigned char> body_vec = task.get();

		for (auto& i : body_vec) {
			std::cout << i;
		}
	

	});

	// wait for all the outstanding i/o to complete and handle any exceptions.
	try {
		
	}

	// catch block.
	catch (const std::exception &e) {
		printf("error exception:%s\n", e.what());
	};

	// Exit.
	std::cin.get();
	return 0;
}