#ifndef QMLDEBTORMODEL_H
#define QMLDEBTORMODEL_H

#include <QObject>
#include <QVariantList>
#include "models/abstractvisuallistmodel.h"

class QMLDebtorModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum SuccessCode {
        UnknownSuccess,
        ViewDebtorsSuccess,
        UndoRemoveDebtorSuccess,
        RemoveDebtorSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError,
        InvalidDebtorError
    }; Q_ENUM(ErrorCode)

    enum Roles {
        ClientIdRole = Qt::UserRole,
        DebtorIdRole,
        ImageUrlRole,
        PreferredNameRole,
        TotalDebtRole,
        NoteRole,
        CreatedRole,
        LastEditedRole,
        UserRole
    };

    enum FilterColumn {
        PreferredNameColumn, TotalDebtColumn
    }; Q_ENUM(FilterColumn)

    explicit QMLDebtorModel(QObject *parent = nullptr);
    explicit QMLDebtorModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
    void filter() override final;
public slots:
    void removeDebtor(int debtorId);
private:
    QVariantList m_records;

    int debtorRowFromId(int debtorId);
    QString filterColumnName() const;

    void removeItemFromModel(int debtorId);
    void undoRemoveItemFromModel(int row, int debtorId, const QVariantMap &debtorInfo);
};

#endif // QMLDEBTORMODEL_H
