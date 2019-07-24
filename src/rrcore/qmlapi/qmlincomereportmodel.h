#ifndef QMLINCOMEREPORTMODEL_H
#define QMLINCOMEREPORTMODEL_H

#include "models/abstractvisualtablemodel.h"

class QMLIncomeReportModel : public AbstractVisualTableModel
{
public:
    enum Roles {
        PurposeRole = Qt::UserRole,
        AmountRole
    };

    enum SuccessCode {
        ViewIncomeReportSuccess
    }; Q_ENUM(SuccessCode)

    explicit QMLIncomeReportModel(QObject *parent = nullptr);
    explicit QMLIncomeReportModel(DatabaseThread &thread, QObject *parent = nullptr);

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

#endif // QMLINCOMEREPORTMODEL_H
