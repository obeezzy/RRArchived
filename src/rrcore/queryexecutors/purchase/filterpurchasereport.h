#ifndef FILTERPURCHASEREPORT_H
#define FILTERPURCHASEREPORT_H

#include "purchaseexecutor.h"

class FilterCriteria;
class SortCriteria;
class DateTimeSpan;

class FilterPurchaseReport : public PurchaseExecutor
{
    Q_OBJECT
public:
    inline static const QString COMMAND = QStringLiteral("filter_purchase_report");

    explicit FilterPurchaseReport(const FilterCriteria &filterCriteria,
                                  const SortCriteria &sortCriteria,
                                  const DateTimeSpan &dateTimeSpan,
                                  QObject *receiver);
    QueryResult execute() override;
};

#endif // FILTERPURCHASEREPORT_H
