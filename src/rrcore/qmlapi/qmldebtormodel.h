#ifndef QMLDEBTORMODEL_H
#define QMLDEBTORMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/debtor/debtor.h"

class QMLDebtorModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum SuccessCode {
        ViewDebtorsSuccess,
        UndoRemoveDebtorSuccess,
        RemoveDebtorSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
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
    QString columnName(int column) const override final;
public slots:
    void removeDebtor(int row);
private:
    Utility::DebtorList m_debtorList;

    void removeDebtorFromModel(const Utility::Debtor &debtor);
    void undoRemoveDebtorFromModel(const Utility::Debtor &debtor);
};

#endif // QMLDEBTORMODEL_H
