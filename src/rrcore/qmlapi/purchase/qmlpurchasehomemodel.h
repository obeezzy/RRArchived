#ifndef QMLPURCHASEHOMEMODEL_H
#define QMLPURCHASEHOMEMODEL_H

#include "models/abstracthomemodel.h"

class AbstractVisualListModel;

class QMLPurchaseHomeModel : public AbstractHomeModel
{
public:
    explicit QMLPurchaseHomeModel(QObject *parent = nullptr);
    explicit QMLPurchaseHomeModel(DatabaseThread &thread,
                                  QObject *parent = nullptr);
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override;
    void processResult(const QueryResult &result) override;
private:
    QList<AbstractVisualListModel *> m_dataModels;
};

#endif // QMLPURCHASEHOMEMODEL_H
