#include "vendor.h"

using namespace Utility;

Vendor::Vendor(const Client &client) :
    client(client)
{}

Vendor::Vendor(int id,
               const Client &client) :
    id(id),
    client(client)
{}

Vendor::Vendor(const QVariantMap &map) :
    id(map.value("vendor_id").toInt()),
    client(Client{ map })
{}
