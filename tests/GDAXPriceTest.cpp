#include "GDAXPrice.h"


#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <string>
#include <iostream>

TEST_CASE("GDAX Price tests") {
	utility::string_t coin;
	double price;

	SECTION("Etherium") {
		coin = U("ETH");
		price = Apollo::getAskingPriceGdax(coin);
		REQUIRE(price != 0);
	}

	SECTION("Litecoin") {
		coin = U("LTC");
		price = Apollo::getAskingPriceGdax(coin);
		REQUIRE(price != 0);
	}
	SECTION("Bitcoin") {
		coin = U("BTC");
		price = Apollo::getAskingPriceGdax(coin);
		REQUIRE(price != 0);
	}

	SECTION("Bitcoin Cash") {
		coin = U("BCH");
		price = Apollo::getAskingPriceGdax(coin);
		REQUIRE(price != 0);
	}

}