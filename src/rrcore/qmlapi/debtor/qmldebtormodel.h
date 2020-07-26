#ifndef QMLDEBTORMODEL_H
#define QMLDEBTORMODEL_H

#include <QLoggingCategory>
#include "models/abstractvisuallistmodel.h"
#include "utility/debtor/debtor.h"

class QMLDebtorModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum SuccessCode
    {
        ViewDebtorsSuccess,
        UndoRemoveDebtorSuccess,
        RemoveDebtorSuccess
    };
    Q_ENUM(SuccessCode)

    enum ErrorCode
    {
        InvalidDebtorError
    };
    Q_ENUM(ErrorCode)

    enum Roles
    {
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

    enum FilterColumn
    {
        PreferredNameColumn,
        TotalDebtColumn
    };
    Q_ENUM(FilterColumn)

    explicit QMLDebtorModel(QObject* parent = nullptr);
    explicit QMLDebtorModel(DatabaseThread& thread, QObject* parent = nullptr);

    int rowCount(
        const QModelIndex& parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    Q_INVOKABLE void removeDebtor(int row);

protected:
    void tryQuery() override final;
    bool canProcessResult(const QueryResult& result) const override;
    void processResult(const QueryResult& result) override final;
    void filter() override final;
    QString columnName(int column) const override final;
public slots:
    void undoLastCommit() override;

private:
    Utility::Debtor::DebtorList m_debtorList;

    void removeDebtorFromModel(const Utility::Debtor::Debtor& debtor);
    void undoRemoveDebtorFromModel(const Utility::Debtor::Debtor& debtor);
};

Q_DECLARE_LOGGING_CATEGORY(lcqmldebtormodel);

#endif  // QMLDEBTORMODEL_H
