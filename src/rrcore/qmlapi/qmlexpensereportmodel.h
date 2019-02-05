#ifndef EXPENSEREPORTMODEL_H
#define EXPENSEREPORTMODEL_H

#include "models/abstractvisualtablemodel.h"

class QMLExpenseReportModel : public AbstractVisualTableModel
{
    Q_OBJECT
public:
    enum Roles {
        PurposeRole = Qt::UserRole,
        AmountRole
    };

    enum SuccessCode {
        ViewExpenseReportSuccess
    }; Q_ENUM(SuccessCode)

    explicit QMLExpenseReportModel(QObject *parent = nullptr);
    explicit QMLExpenseReportModel(DatabaseThread &thread);

    int rowCount(const QModelIndex &index = QModelIndex()) const override;
    int columnCount(const QModelIndex &index = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
};

#endif // EXPENSEREPORTMODEL_H
