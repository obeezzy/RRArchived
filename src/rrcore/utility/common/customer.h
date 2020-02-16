#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "client.h"

namespace Utility {
struct Customer {
    int id {-1};
    Client client;

    explicit Customer() = default;
    explicit Customer(const Client &client);
    explicit Customer(int id,
                      const Client &client);
};
}

#endif // CUSTOMER_H
