#ifndef QMLEXPENSEHOMEMODEL_H
#define QMLEXPENSEHOMEMODEL_H

#include "models/abstracthomemodel.h"

class QMLExpenseHomeModel : public AbstractHomeModel
{
public:
    explicit QMLExpenseHomeModel(QObject *parent = nullptr);
    explicit QMLExpenseHomeModel(DatabaseThread &thread);
protected:
    void tryQuery() override;
};

#endif // QMLEXPENSEHOMEMODEL_H
