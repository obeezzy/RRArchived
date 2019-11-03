#ifndef QMLUSERMODEL_H
#define QMLUSERMODEL_H

#include "models/abstractvisualtablemodel.h"

class QMLUserModel : public AbstractVisualTableModel
{
    Q_OBJECT
    Q_PROPERTY(int keys READ keys WRITE setKeys NOTIFY keysChanged)
public:
    enum Roles {
        UserIdRole = Qt::UserRole,
        UserRole,
        ActiveRole,
        PresetRole
    };

    enum Columns {
        UserColumn,
        ActiveColumn,
        PresetColumn,
        ActionColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    enum FilterKey {
        None,
        Active,
        Archived,
        All
    }; Q_ENUM(FilterKey)

    enum SuccessCode {
        ViewUsersSuccess,
        RemoveUserSuccess,
        UndoRemoveUserSuccess,
        ActivateUserSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        InsufficientPrivilegeError
    }; Q_ENUM(ErrorCode)

    explicit QMLUserModel(QObject *parent = nullptr);
    explicit QMLUserModel(DatabaseThread &thread, QObject *parent = nullptr);

    int keys() const;
    void setKeys(int keys);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void removeUser(const QString &userName);
    Q_INVOKABLE void activateUser(const QString &userName, bool active);
signals:
    void keysChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
    int m_keys;

    void removeUserFromModel(const QString &userName);
};

#endif // QMLUSERMODEL_H
