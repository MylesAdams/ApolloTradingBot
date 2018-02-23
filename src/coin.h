//file: coin.h
//written by ANDREW LAUX

#ifndef coin_h
#define coin_h
#include <cpprest\http_client.h>

// Define namespace.
using namespace utility;

//Class: Coin
class coin {
public:
	string_t coin_id;
	double amount;
};

#endif // !coin.h

