#include <FourChan.h>
#include <Bot.h>

#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <string>
#include <iostream>

using Apollo::Bot::FourChan;
using namespace web::http;
using utility::string_t;




TEST_CASE("ForChan tests") {
	FourChan fc;

	SECTION("gets assigned, and converts to lowercase") {
		fc.setSearchQuery(" adam Is SUpER CooL yo 345 Q3lk45J2q345jl-  ");
		std::string pattern = fc.getsearch_pattern_();
		REQUIRE(pattern == " adam is super cool yo 345 q3lk45j2q345jl-  ");
	}

	SECTION("gets rid of whitespace") {
		std::vector<Apollo::Comment> comments;
		comments.push_back(Apollo::Comment("adam", "69"));
		comments.push_back(Apollo::Comment(" the meaning of life    ", "42"));
		comments.push_back(Apollo::Comment("highway though sb ", "101"));
		comments.push_back(Apollo::Comment(" waiting in line at the dmv is like being in", "666"));
		comments = fc.getcleanComments(comments);

		REQUIRE(comments[0].content == "adam");
		REQUIRE(comments[1].content == "the meaning of life");
		REQUIRE(comments[2].content == "highway though sb");
		REQUIRE(comments[3].content == "waiting in line at the dmv is like being in");
	}

	SECTION("parse JSON") {
		std::string json = "{\"hello\": \"world\",\"t\" : true,\"f\" : false,\"n\" : null,\"i\" : 123,\"pi\" : 3.1416,\"a\" : [1, 2, 3, 4]}";
		rapidjson::Document document;
		document.Parse(json);


	}
}
