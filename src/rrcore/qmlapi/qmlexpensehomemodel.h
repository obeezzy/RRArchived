#ifndef QMLEXPENSEHOMEMODEL_H
#define QMLEXPENSEHOMEMODEL_H

#include "models/abstracthomemodel.h"

class QMLExpenseHomeModel : public AbstractHomeModel
{
public:
    explicit QMLExpenseHomeModel(QObject *parent = nullptr);
    explicit QMLExpenseHomeModel(DatabaseThread &thread,
                                 QObject *parent = nullptr);
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override;
    void processResult(const QueryResult &result) override;
};

#endif // QMLEXPENSEHOMEMODEL_H
