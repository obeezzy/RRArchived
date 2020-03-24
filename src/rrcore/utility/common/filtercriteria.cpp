#include "filtercriteria.h"

using namespace Utility;

FilterCriteria::FilterCriteria(const QString &column,
                               const QString &text) :
    column(column),
    text(text)
{ }
