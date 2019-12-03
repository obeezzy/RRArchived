#include "viewdebtors.h"

using namespace DebtorQuery;

ViewDebtors::ViewDebtors(QObject *receiver) :
    DebtorExecutor(COMMAND, { }, receiver)
{

}

ViewDebtors::ViewDebtors(const QString &filterText,
                         const QString &filterColumn,
                         QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "filter_text", filterText },
                        { "filter_column", filterColumn }
                   }, receiver)
{

}

QueryResult ViewDebtors::execute()
{
    QueryResult result{ request() };

    return result;
}
