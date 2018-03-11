#include <watson.h>


#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <string>
#include <iostream>



TEST_CASE("ForChan tests") {
	// all the watson stuff for the tests
	Apollo::Watson* watsonAnalyzer;
	utility::string_t WATSON_USERNAME = U("c032fda0-5c02-490d-8e00-ab00de2e5f40");
	utility::string_t WATSON_PASSWORD = U("AfgP2LQIDCgB");
	watsonAnalyzer = new Apollo::Watson(WATSON_USERNAME, WATSON_PASSWORD);
	
	
	SECTION("toneToString") {
		std::string answer = "{\"document_tone\":{\"tones\":[{\"score\":0.807105,\"tone_id\":\"joy\",\"tone_name\":\"Joy\"}]}}";
		std::string given = utility::conversions::to_utf8string(watsonAnalyzer->toneToString(U("i love you, but I have a raid on wow tonight, and that takes priority over our date")));
		REQUIRE(given == answer);
	}
	SECTION("toneToFile") {
		// creating file
		std::string filename = "../resources/watsonTest.json";
		utility::string_t tone_input = U("rawwr, i love pizza so much");
		watsonAnalyzer->toneToFile(tone_input, U("../resources/watsonTest.json"));

		//reading file
		std::ifstream ifs("../resources/watsonTest.json");
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		REQUIRE(content.substr(0, 37) == "[{\"PosRating\":0.90463,\"NegRating\":0.0");
	}
}