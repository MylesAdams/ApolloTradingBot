// File: watson.cpp
// Written by ANDREW LAUX

#include "watson.h"

// Constructor for watson object.
Apollo::Watson::Watson(utility::string_t user, utility::string_t pass) :
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
utility::string_t Apollo::Watson::toneToString(const utility::string_t & tone_input) {

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
web::json::value Apollo::Watson::toneToJson(const utility::string_t & tone_input) {

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

	// Return obj
	return json_obj;	
}

// Function writes tone sentiment to file.
void Apollo::Watson::toneToFile(const utility::string_t & tone_input, const utility::string_t & file_name) {

	// Call toneToJson to get json object.
	web::json::value json_sentiment = toneToJson(tone_input);

	// declare variables to track negative and positive ratings.
	double positive_rating = 0;
	double negative_rating = 0;
	double count_neg = 0;
	double count_pos = 0;

	// Loop through tones given by json returned by watson.
	for (size_t i = 0; i < json_sentiment[U("document_tone")][U("tones")].size(); i++) {

		// Collect sentiment values.
		auto value_tone = json_sentiment[U("document_tone")][U("tones")][i][U("tone_name")];
		auto value_score = json_sentiment[U("document_tone")][U("tones")][i][U("score")];
		double score = value_score.as_double();
		utility::string_t tone = value_tone.as_string();

		// Get rating.
		double rating = score * rateTone(tone);

		// Increment positive. 
		if (rating > 0) {
			positive_rating += rating;
			count_pos++;
		}

		// Increment negative.
		else {
			negative_rating += rating;
			count_neg++;
		}

	}

	// Average ratings. 
	positive_rating = positive_rating / count_pos;
	negative_rating = -1*(negative_rating / count_neg);

	// TODO: Throw logical error if ratings are < 0.5 or >1 

	// Get timestamp.
	int utc = static_cast<int>(this->date.utc_timestamp());

	// Open outgoing file stream.
	std::ifstream check_file(file_name);

	// Create json doc
	rapidjson::Document doc;

	// File already exists.
	if (check_file.good()) {

		// Wrap std::ifstream object.
		rapidjson::IStreamWrapper isw(check_file);
		doc.ParseStream(isw);
	}

	// File doesn't exist.
	else doc.SetArray();

	//Allocator
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	// Generate entry.
	rapidjson::Value v;
	v.SetObject();
	v.AddMember("PosRating", rapidjson::Value(positive_rating), allocator);
	v.AddMember("NegRating", rapidjson::Value(negative_rating), allocator);
	v.AddMember("Time", rapidjson::Value(utc), allocator);
	doc.PushBack(v, allocator);

	// Use json writer to output to file.
	std::ofstream out(file_name);
	rapidjson::OStreamWrapper osw(out);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
	doc.Accept(writer);
	out.close();

}

// Helper function rates tone. 1 for pos, 0 for neutral, -1 for neg.
int Apollo::Watson::rateTone(const utility::string_t & tone) {

	//Rating
	int rating = 0;

	// Positive tones.
	if (tone == U("Satisfied")) rating = 1;
	else if (tone == U("Excited")) rating = 1;
	else if (tone == U("Joy")) rating = 1;
	else if (tone == U("Confident")) rating = 1;

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
