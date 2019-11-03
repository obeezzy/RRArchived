#ifndef USERPRIVILEGEMODEL_H
#define USERPRIVILEGEMODEL_H

#include "models/abstractvisuallistmodel.h"

class UserPrivilegeModel : public AbstractVisualListModel
{
public:
    enum Roles {
        KeyRole = Qt::UserRole,
        ShortDescriptionRole,
        LongDescriptionRole,
        ValueRole
    };
    explicit UserPrivilegeModel(QObject *parent = nullptr);
    explicit UserPrivilegeModel(DatabaseThread &thread);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setPrivileges(const QString &privilegeGroup, const QString &title, const QVariantList &privileges);
    void setPrivilegeValue(int row, bool value);

    QString privilegeGroup() const;
    QString title() const;
    QVariantList privileges() const;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
    QString m_privilegeGroup;
    QString m_title;
};

#endif // USERPRIVILEGEMODEL_H
