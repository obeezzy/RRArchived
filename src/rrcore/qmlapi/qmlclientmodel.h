#ifndef QMLCLIENTMODEL_H
#define QMLCLIENTMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/common/client.h"

class QMLClientModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Role {
        ClientIdRole = Qt::UserRole,
        PreferredNameRole,
        PhoneNumberRole
    };

    enum FilterColumn {
        PreferredNameColumn,
        PhoneNumberColumn
    }; Q_ENUM(FilterColumn)

    enum SuccessCode {
        UnknownSuccess,
        ViewClientsSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError
    }; Q_ENUM(ErrorCode)

    explicit QMLClientModel(QObject *parent = nullptr);
    explicit QMLClientModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
    void filter() override final;
    QString columnName(int column) const override;
private:
    Utility::ClientList m_clientList;

};

#endif // QMLCLIENTMODEL_H
