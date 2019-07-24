#ifndef ABSTRACTHOMEMODEL_H
#define ABSTRACTHOMEMODEL_H

#include <QObject>
#include "abstractvisuallistmodel.h"

class DatabaseThread;

class AbstractHomeModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Role {
        TitleRole = Qt::UserRole,
        ImageUrlRole,
        IconUrlRole,
        ShortDescriptionRole,
        LongDescriptionRole,
        BreadcrumbsRole,
        ChartTypeRole,
        ChartModelRole
    };
    explicit AbstractHomeModel(QObject *parent = nullptr);
    explicit AbstractHomeModel(DatabaseThread &thread, QObject *parent = nullptr);
    virtual ~AbstractHomeModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override;
protected:
    QVariantList records() const;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
};

#endif // ABSTRACTHOMEMODEL_H
