#include "exchange_accnt.h"

std::vector<Apollo::Exchanges::Coin> Apollo::Exchanges::ExchangeAccnt::getHoldings()
{
    this->connect();
    this->update();
    std::vector<Apollo::Exchanges::Coin> holdings;
    for (auto& coin : this->coins_vec)
        if (coin.amount > 0)
            holdings.push_back(coin);
    return holdings;
}
