#ifndef SORTCRITERIA_H
#define SORTCRITERIA_H

#include <QString>

namespace Utility {
struct SortCriteria {
    QString column;
    Qt::SortOrder order {Qt::AscendingOrder};
    int columnAsInteger {0};

    explicit SortCriteria() = default;
    explicit SortCriteria(Qt::SortOrder order);
    explicit SortCriteria(const QString &column,
                          Qt::SortOrder order);

    QString orderAsString() const;

    inline bool isValid() const {
        return !column.isEmpty() && columnAsInteger > -1;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::SortCriteria, Q_PRIMITIVE_TYPE);

#endif // SORTCRITERIA_H
