#include "sortcriteria.h"

using namespace Utility;

SortCriteria::SortCriteria(Qt::SortOrder order) : order(order) {}

SortCriteria::SortCriteria(const QString& column, Qt::SortOrder order)
    : column(column),
      order(order)
{}

QString SortCriteria::orderAsString() const
{
    return order == Qt::DescendingOrder ? QStringLiteral("descending")
                                        : QStringLiteral("ascending");
}
