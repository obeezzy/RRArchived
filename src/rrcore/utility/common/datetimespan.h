#ifndef DATETIMESPAN_H
#define DATETIMESPAN_H

#include <QDateTime>

namespace Utility {
struct DateTimeSpan {
    QDateTime from;
    QDateTime to;

    explicit DateTimeSpan() = default;
    explicit DateTimeSpan(const QDateTime &from,
                          const QDateTime &to);
};
}

#endif // DATETIMESPAN_H
