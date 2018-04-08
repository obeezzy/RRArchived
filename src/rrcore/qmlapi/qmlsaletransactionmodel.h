#ifndef QMLSALETRANSACTIONMODEL_H
#define QMLSALETRANSACTIONMODEL_H

#include "models/abstractvisuallistmodel.h"

class QMLSaleTransactionModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int transactionId READ transactionId WRITE setTransactionId NOTIFY transactionIdChanged)
    Q_PROPERTY(int keys READ keys WRITE setKeys NOTIFY keysChanged)
public:
    enum FilterKeys {
        None,
        Completed,
        Suspended,
        Archived,
        All
    }; Q_ENUM(FilterKeys)

    enum Roles {
        TransactionIdRole = Qt::UserRole,
                ClientIdRole,
                CustomerNameRole,
                TotalCostRole,
                AmountPaidRole,
                BalanceRole,
                DiscountRole,
                NoteIdRole,
                NoteRole,
                SuspendedRole,
                ArchivedRole,
                CreatedRole,
                LastEditedRole,
                UserIdRole
    };

    QMLSaleTransactionModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    int transactionId() const;
    void setTransactionId(int transactionId);

    int keys() const;
    void setKeys(int keys);
signals:
    void transactionIdChanged();
    void keysChanged();
protected:
    void tryQuery() override final;
    void processResult(const QueryResult &result) override final;
private:
    QVariantList m_records;
    int m_transactionId;
    int m_keys;
};

#endif // QMLSALETRANSACTIONMODEL_H
