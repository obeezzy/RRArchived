#ifndef QMLINCOMEHOMEMODEL_H
#define QMLINCOMEHOMEMODEL_H

#include "models/abstracthomemodel.h"

class QMLIncomeHomeModel : public AbstractHomeModel
{
    Q_OBJECT
public:
    explicit QMLIncomeHomeModel(QObject *parent = nullptr);
    explicit QMLIncomeHomeModel(DatabaseThread &thread,
                                QObject *parent = nullptr);
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override;
    void processResult(const QueryResult &result) override;
};

#endif // QMLINCOMEHOMEMODEL_H
