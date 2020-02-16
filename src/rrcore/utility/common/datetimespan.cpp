#include "datetimespan.h"

using namespace Utility;

DateTimeSpan::DateTimeSpan(const QDateTime &from,
                           const QDateTime &to) :
    from(from),
    to(to)
{ }
