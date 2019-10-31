#ifndef QMLCREDITORMODEL_H
#define QMLCREDITORMODEL_H

#include "models/abstractvisuallistmodel.h"

class QMLCreditorModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum SuccessCode {
        UnknownSuccess,
        ViewCreditorsSuccess,
        UndoRemoveCreditorSuccess,
        RemoveCreditorSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError,
        InvalidCreditorError
    }; Q_ENUM(ErrorCode)

    enum Roles {
        ClientIdRole = Qt::UserRole,
        CreditorIdRole,
        ImageSourceRole,
        PreferredNameRole,
        TotalCreditRole,
        NoteRole,
        CreatedRole,
        LastEditedRole,
        UserRole
    };

    enum FilterColumn {
        PreferredNameColumn, TotalCreditColumn
    }; Q_ENUM(FilterColumn)

    explicit QMLCreditorModel(QObject *parent = nullptr);
    explicit QMLCreditorModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
};

#endif // QMLCREDITORMODEL_H
