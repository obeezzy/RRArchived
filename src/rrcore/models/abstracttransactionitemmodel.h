#ifndef ABSTRACTTRANSACTIONITEMMODEL_H
#define ABSTRACTTRANSACTIONITEMMODEL_H

#include "abstractvisualtablemodel.h"

class AbstractTransactionItemModel : public AbstractVisualTableModel
{
    Q_OBJECT
    Q_PROPERTY(int transactionId READ transactionId WRITE setTransactionId NOTIFY transactionIdChanged)
public:
    explicit AbstractTransactionItemModel(QObject *parent = nullptr);
    explicit AbstractTransactionItemModel(DatabaseThread &thread, QObject *parent = nullptr);

    int transactionId() const;
    void setTransactionId(int transactionId);
signals:
    void transactionIdChanged();
private:
    int m_transactionId;
};

#endif // ABSTRACTTRANSACTIONITEMMODEL_H
