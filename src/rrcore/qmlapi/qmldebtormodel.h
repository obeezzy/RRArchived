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
        PreferredNameRole,
        TotalDebtRole,
        NoteRole,
        CreatedRole,
        LastEditedRole,
        UserRole
    };

    enum Column {
        PreferredNameColumn, TotalDebtColumn
    }; Q_ENUM(Column)

    explicit QMLDebtorModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
    void filter() override final;
public slots:
    void refresh();
    void removeDebtor(int debtorId);
private:
    QVariantList m_records;

    void removeItemFromModel(int debtorId);
    void undoRemoveItemFromModel(int debtorId, const QVariantMap &debtorInfo);
};

#endif // QMLDEBTORMODEL_H
