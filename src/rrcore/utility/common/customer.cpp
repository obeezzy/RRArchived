#include "customer.h"

using namespace Utility;

Customer::Customer(const Client& client) : client(client) {}

Customer::Customer(int id, const Client& client) : id(id), client(client) {}
