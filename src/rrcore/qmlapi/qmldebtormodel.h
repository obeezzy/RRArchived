#ifndef QMLDEBTORMODEL_H
#define QMLDEBTORMODEL_H

#include <QObject>
#include <QVariantList>
#include "models/abstractvisuallistmodel.h"

class QMLDebtorModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles {
        ClientIdRole = Qt::UserRole,
        NameRole,
        TotalBalanceRole,
        NoteRole,
        CreatedRole,
        LastEditedRole,
        UserRole
    };

    explicit QMLDebtorModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

protected:
    void tryQuery() override final;
    void processResult(const QueryResult &result) override final;
    void filter() override final;
private:
    QVariantList m_records;
};

#endif // QMLDEBTORMODEL_H
