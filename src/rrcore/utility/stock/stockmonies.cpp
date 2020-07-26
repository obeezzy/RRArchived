#include "stockmonies.h"

using namespace Utility::Stock;

StockMonies::StockMonies()
{}

StockMonies::StockMonies(const QVariantMap &map) :
    costPrice(map.value("cost_price").toDouble()),
    retailPrice(map.value("retail_price").toDouble()),
    unitPrice(map.value("unit_price").toDouble())
{}
