// File: watson.cpp
// Written by ANDREW LAUX

#include "watson.h"

// Constructor for watson object.
Watson::Watson(utility::string_t user, utility::string_t pass) :
	creds(user, pass),
	config(),
	builder(U("/v3/tone")),
	req(web::http::methods::POST) {

	// Set config.
	config.set_credentials(creds);

	// Set request uri with builder to string.
	builder.append_query(U("version"), U("2017-09-21"));
	builder.append_query(U("sentences"), U("false"));
	req.set_request_uri(builder.to_string());
}

// Function: Returns sentiment as stringified JSON.
utility::string_t Watson::toneToString(const utility::string_t & tone_input) {

	// String to hold sentiment.
	utility::string_t sentiment_str;

	web::json::value json_obj;

	// Create watson_client.
	web::http::client::http_client watson_client(U("https://gateway.watsonplatform.net/tone-analyzer/api/"), this->config);

	// Set request body to tone_input.
	this->req.set_body(tone_input);

	// Attempt request.
	pplx::task<web::http::http_response> watson_call = watson_client.request(req);
	watson_call

		// Hookup response continuation
		.then([](web::http::http_response response) {

		// Report error.
		if (response.status_code() != web::http::status_codes::OK) {
			// TODO: throw exception
		}

		// Extract body into vector.
		return response.extract_string();
	    })

		// Hookup continuation to process body.
		.then([&sentiment_str](pplx::task<utility::string_t> string_task) {
		
		//Process extracted json.
		sentiment_str = string_task.get();

		// Collect outstanding tasks.
	    }).wait(); 


	return sentiment_str;
}

// Function: Outputs sentiment to <coinID>.josn.
void Watson::toneRatingToFile(const utility::string_t & tone_input, const utility::string_t & filename) {

	// Response json.
	web::json::value json_obj;

	// Create watson_client.
	web::http::client::http_client watson_client(U("https://gateway.watsonplatform.net/tone-analyzer/api/"), this->config);

	// Set request body to tone_input.
	this->req.set_body(tone_input);

	// Attempt request.
	pplx::task<web::http::http_response> watson_call = watson_client.request(req);
	watson_call

	// Hookup response continuation
	.then([](web::http::http_response response) {

		// Report error.
		if (response.status_code() != web::http::status_codes::OK) {
			// TODO: throw exception
		}

		// Extract body into vector.
		return response.extract_json();
	})

	// Hookup continuation to process body.
	.then([&json_obj](pplx::task<web::json::value> json_task) {

		//Process extracted json.
		json_obj = json_task.get();

	}).wait(); // Collect outstanding tasks.


	// Collect sentiment values.
	auto value_tone = json_obj[U("document_tone")][U("tones")][0][U("tone_name")];
	auto value_score = json_obj[U("document_tone")][U("tones")][0][U("score")];
	double score = value_score.as_double();
	utility::string_t tone = value_tone.as_string();

	// Get timestamp.
	int utc = static_cast<int>(this->date.utc_timestamp());

	// Get index. Multiply score by rating.
	double rating = score * rateTone(tone);

	// Open outgoing file stream.
	utility::ofstream_t outfile;
	outfile.open(filename);

	// Check that stream is open
	if (outfile.fail()) {
		//TODO: throw exception
	}
	
}
// Helper function rates tone. 1 for pos, 0 for neutral, -1 for neg.
int Watson::rateTone(const utility::string_t & tone) {

	//Rating
	int rating = 0;

	// Positive tones.
	if (tone == U("Satisfied")) rating = 1;
	else if (tone == U("Excited")) rating = 1;
	else if (tone == U("Joy")) rating = 1;
	else if (tone == U("")) rating = 1;

	// Neutral tones.
	else if (tone == U("Polite")) rating = 0;

	// Negative tones.
	else if (tone == U("Sad")) rating = -1;
	else if (tone == U("Frustrated")) rating = -1;
	else if (tone == U("Impolite")) rating = -1;
	else if (tone == U("Sympathetic")) rating = -1;
	else if (tone == U("fear")) rating = -1;
	else if (tone == U("Tentative")) rating = -1;

	// Unknown sentiment.
	return rating;
}
