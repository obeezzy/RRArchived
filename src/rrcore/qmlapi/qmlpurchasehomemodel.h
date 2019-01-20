#ifndef QMLPURCHASEHOMEMODEL_H
#define QMLPURCHASEHOMEMODEL_H

#include "models/abstracthomemodel.h"

class AbstractVisualListModel;

class QMLPurchaseHomeModel : public AbstractHomeModel
{
public:
    explicit QMLPurchaseHomeModel(QObject *parent = nullptr);
    explicit QMLPurchaseHomeModel(DatabaseThread &thread);

protected:
    void tryQuery() override;
private:
    QList<AbstractVisualListModel *> m_dataModels;

};

#endif // QMLPURCHASEHOMEMODEL_H
