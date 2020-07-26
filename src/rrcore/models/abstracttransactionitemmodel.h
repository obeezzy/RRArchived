#ifndef ABSTRACTTRANSACTIONITEMMODEL_H
#define ABSTRACTTRANSACTIONITEMMODEL_H

#include "abstractvisualtablemodel.h"

class AbstractTransactionItemModel : public AbstractVisualTableModel
{
    Q_OBJECT
    Q_PROPERTY(int transactionId READ transactionId WRITE setTransactionId
                   NOTIFY transactionIdChanged)
public:
    explicit AbstractTransactionItemModel(QObject* parent = nullptr);
    explicit AbstractTransactionItemModel(DatabaseThread& thread,
                                          QObject* parent = nullptr);

    int transactionId() const;
    void setTransactionId(int transactionId);

    bool isExistingTransaction() const;
signals:
    void transactionIdChanged();

private:
    int m_transactionId{0};
};

#endif  // ABSTRACTTRANSACTIONITEMMODEL_H
