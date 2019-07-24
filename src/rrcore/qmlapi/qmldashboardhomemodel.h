#ifndef QMLDASHBOARDHOMEMODEL_H
#define QMLDASHBOARDHOMEMODEL_H

#include <QObject>
#include "models/abstracthomemodel.h"

class QMLDashboardHomeModel : public AbstractHomeModel
{
    Q_OBJECT
public:
    explicit QMLDashboardHomeModel(QObject *parent = nullptr);
    explicit QMLDashboardHomeModel(DatabaseThread &thread, QObject *parent = nullptr);
protected:
    void tryQuery() override;
};

#endif // QMLDASHBOARDHOMEMODEL_H
