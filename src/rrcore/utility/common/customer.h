#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "client.h"

namespace Utility {
namespace Common {
struct Customer
{
    int id{0};
    Client client;

    explicit Customer() = default;
    explicit Customer(const Client& client);
    explicit Customer(int id, const Client& client = Client());

    friend QDebug operator<<(QDebug debug, const Customer& customer)
    {
        debug.nospace() << "Customer("
                        << "id=" << customer.id
                        << ", client=" << customer.client << ")";

        return debug.nospace();
    }
};
}  // namespace Common
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Customer, Q_PRIMITIVE_TYPE);

#endif  // CUSTOMER_H
