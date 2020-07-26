#ifndef QMLTOTALREVENUEMODEL_H
#define QMLTOTALREVENUEMODEL_H

#include <QVariantList>
#include "models/abstractvisuallistmodel.h"

class DatabaseThread;
class QMLTotalRevenueModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        CreatedRole = Qt::UserRole,
        AmountPaidRole
    };

    explicit QMLTotalRevenueModel(QObject* parent = nullptr);
    explicit QMLTotalRevenueModel(DatabaseThread& thread,
                                  QObject* parent = nullptr);

    int rowCount(
        const QModelIndex& parent = QModelIndex()) const override final;
    int columnCount(
        const QModelIndex& parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

protected:
    void tryQuery() override final;
    bool canProcessResult(const QueryResult& result) const override;
    void processResult(const QueryResult& result) override final;

private:
    QVariantList m_records;
};

#endif  // QMLTOTALREVENUEMODEL_H
