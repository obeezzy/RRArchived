#ifndef QMLINCOMEREPORTMODEL_H
#define QMLINCOMEREPORTMODEL_H

#include "models/abstractreportmodel.h"
#include "utility/incomeutils.h"

class QMLIncomeReportModel : public AbstractReportModel
{
    Q_OBJECT
public:
    enum Roles {
        PurposeRole = Qt::UserRole,
        AmountRole
    };

    enum SuccessCode {
        ViewIncomeReportSuccess
    }; Q_ENUM(SuccessCode)

    enum Columns {
        PurposeColumn,
        AmountColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    explicit QMLIncomeReportModel(QObject *parent = nullptr);
    explicit QMLIncomeReportModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &index = QModelIndex()) const override;
    int columnCount(const QModelIndex &index = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    Utility::IncomeReportTransactionList m_transactions;
};

#endif // QMLINCOMEREPORTMODEL_H
