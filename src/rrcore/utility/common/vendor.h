#ifndef VENDOR_H
#define VENDOR_H

#include "client.h"

namespace Utility {
struct Vendor {
    int id {-1};
    Client client;

    explicit Vendor() = default;
    explicit Vendor(const Client &client);
    explicit Vendor(int id,
                    const Client &client);
};
}
Q_DECLARE_TYPEINFO(Utility::Vendor, Q_PRIMITIVE_TYPE);

#endif // VENDOR_H
