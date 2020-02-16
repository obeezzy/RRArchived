#ifndef FILTERCRITERIA_H
#define FILTERCRITERIA_H

#include <QString>

namespace Utility {
struct FilterCriteria {
    QString column;
    QString text;
    int columnAsInteger {-1};

    explicit FilterCriteria() = default;
    explicit FilterCriteria(const QString &text);
    explicit FilterCriteria(const QString &column,
                            const QString &text);

    inline bool isValid() const {
        return columnAsInteger > -1
                && !column.isEmpty()
                && !text.isEmpty();
    }
};
}
#endif // FILTERCRITERIA_H
