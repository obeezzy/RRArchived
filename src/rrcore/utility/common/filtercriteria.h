#ifndef FILTERCRITERIA_H
#define FILTERCRITERIA_H

#include <QString>

namespace Utility {
struct FilterCriteria
{
    QString column;
    QString text;
    int columnIndex{-1};

    explicit FilterCriteria() = default;
    explicit FilterCriteria(const QString& column, const QString& text);

    inline bool isValid() const
    {
        return columnIndex > -1 && !column.isEmpty() && !text.isEmpty();
    }
};
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::FilterCriteria, Q_PRIMITIVE_TYPE);

#endif  // FILTERCRITERIA_H
