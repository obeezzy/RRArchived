#ifndef QMLDASHBOARDHOMEMODEL_H
#define QMLDASHBOARDHOMEMODEL_H

#include <QObject>
#include "models/abstracthomemodel.h"

class QMLDashboardHomeModel : public AbstractHomeModel
{
    Q_OBJECT
public:
    explicit QMLDashboardHomeModel(QObject* parent = nullptr);
    explicit QMLDashboardHomeModel(DatabaseThread& thread,
                                   QObject* parent = nullptr);

protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult& result) const override;
    void processResult(const QueryResult& result) override;
};

#endif  // QMLDASHBOARDHOMEMODEL_H
