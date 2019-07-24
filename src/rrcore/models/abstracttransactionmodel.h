#ifndef ABSTRACTTRANSACTIONMODEL_H
#define ABSTRACTTRANSACTIONMODEL_H

#include "abstractvisualtablemodel.h"
#include <QDateTime>

class AbstractTransactionModel : public AbstractVisualTableModel
{
    Q_OBJECT
    Q_PROPERTY(int keys READ keys WRITE setKeys NOTIFY keysChanged)
    Q_PROPERTY(QDateTime from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QDateTime to READ to WRITE setTo NOTIFY toChanged)
public:
    enum FilterKey {
        None,
        Completed,
        Suspended,
        Archived,
        All
    }; Q_ENUM(FilterKey)

    explicit AbstractTransactionModel(QObject *parent = nullptr);
    explicit AbstractTransactionModel(DatabaseThread &thread, QObject *parent = nullptr);
    virtual ~AbstractTransactionModel() override;

    int keys() const;
    void setKeys(int keys);

    QDateTime from() const;
    void setFrom(const QDateTime &from);

    QDateTime to() const;
    void setTo(const QDateTime &to);

    void componentComplete() override;
signals:
    void transactionIdChanged();
    void keysChanged();
    void fromChanged();
    void toChanged();
private:
    int m_transactionId;
    int m_keys;
    QDateTime m_from;
    QDateTime m_to;

    void toggleConnections();
};

#endif // ABSTRACTTRANSACTIONMODEL_H
