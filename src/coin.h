//file: coin.h
//written by ANDREW LAUX

#ifndef coin_h
#define coin_h
#include <cpprest/http_client.h>

//Define namespace Apollo::Exchanges::Coin
namespace Apollo {
    namespace Exchanges {

        //Class: Coin
        class Coin {

        public: // Data members.
            utility::string_t coin_id;
            double amount;
        };
    }
}

#endif // !coin.h

