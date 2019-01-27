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
        RowNumberRole,
        UserRole,
        ActiveRole,
        PresetRole
    };

    enum FilterKey {
        None,
        Active,
        Archived,
        All
    }; Q_ENUM(FilterKey)

    enum SuccessCode {
        RemoveUserSuccess,
        UndoRemoveUserSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        InsufficientPrivilegeError
    }; Q_ENUM(ErrorCode)

    explicit QMLUserModel(QObject *parent = nullptr);
    explicit QMLUserModel(DatabaseThread &thread);

    int keys() const;
    void setKeys(int keys);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void removeUser(int userId);
signals:
    void keysChanged();
public slots:
    void refresh();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
    int m_keys;
};

#endif // QMLUSERMODEL_H
