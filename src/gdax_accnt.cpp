//File: gdax_accnt.cpp
//Written by ANDREW LAUX

#include"gdax_accnt.h"


// Fucntion: isActive
bool GdaxAccnt::isActive() {

	// Exchange is considered access if credentials are not empty.
	return (!key.empty() && !passphrase.empty() && !secret.empty());
}
