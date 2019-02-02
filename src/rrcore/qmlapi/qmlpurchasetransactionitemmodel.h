#ifndef QMLPURCHASETRANSACTIONITEMMODEL_H
#define QMLPURCHASETRANSACTIONITEMMODEL_H

#include "models/abstracttransactionitemmodel.h"
#include <QVariantList>

class QMLPurchaseTransactionItemModel : public AbstractTransactionItemModel
{
    Q_OBJECT
public:
    enum SuccessCode {
        UnknownSuccess,
        ViewPurchaseTransactionItemsSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError
    }; Q_ENUM(ErrorCode)

    enum Roles {
        TransactionItemIdRole = Qt::UserRole,
        CategoryIdRole,
        CategoryRole,
        ItemIdRole,
        UnitPriceRole,
        ItemRole,
        QuantityRole,
        UnitIdRole,
        UnitRole,
        CostRole,
        DiscountRole,
        CurrencyRole,
        NoteIdRole,
        NoteRole,
        SuspendedRole,
        ArchivedRole,
        CreatedRole,
        LastEditedRole,
        UserIdRole,
        UserRole
    };

    explicit QMLPurchaseTransactionItemModel(QObject *parent = nullptr);
    explicit QMLPurchaseTransactionItemModel(DatabaseThread &thread);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    Q_INVOKABLE void refresh();
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
public slots:
    void removeTransactionItem(int row);
private:
    QVariantList m_records;
};

#endif // QMLPURCHASETRANSACTIONITEMMODEL_H
