#ifndef QMLINCOMEREPORTMODEL_H
#define QMLINCOMEREPORTMODEL_H

#include "models/abstractreportmodel.h"
#include "utility/income/incomereporttransaction.h"

class QMLIncomeReportModel : public AbstractReportModel
{
    Q_OBJECT
public:
    enum Roles
    {
        PurposeRole = Qt::UserRole,
        AmountRole
    };

    enum Columns
    {
        PurposeColumn,
        AmountColumn,
        ColumnCount
    };
    Q_ENUM(Columns)

    explicit QMLIncomeReportModel(QObject* parent = nullptr);
    explicit QMLIncomeReportModel(DatabaseThread& thread,
                                  QObject* parent = nullptr);

    int rowCount(const QModelIndex& index = QModelIndex()) const override;
    int columnCount(const QModelIndex& index = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult& result) const override final;
    void processResult(const QueryResult& result) override final;

private:
    Utility::Income::IncomeReportTransactionList m_transactions;
};

#endif  // QMLINCOMEREPORTMODEL_H
