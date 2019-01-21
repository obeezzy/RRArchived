#ifndef QMLINCOMEPUSHER_H
#define QMLINCOMEPUSHER_H

#include "pusher/abstractpusher.h"

class QMLIncomePusher : public AbstractPusher
{
    Q_OBJECT
    Q_PROPERTY(QString clientName READ clientName WRITE setClientName NOTIFY clientNameChanged)
    Q_PROPERTY(QString purpose READ purpose WRITE setPurpose NOTIFY purposeChanged)
    Q_PROPERTY(qreal amountPaid READ amountPaid WRITE setAmountPaid NOTIFY amountPaidChanged)
public:
    enum class SuccessCode {
        AddIncomeSuccess,
        UpdateIncomeSuccess
    };
    enum class ErrorCode {
        AddIncomeError
    };
    explicit QMLIncomePusher(QObject *parent = nullptr);
    explicit QMLIncomePusher(DatabaseThread &thread);

    QString clientName() const;
    void setClientName(const QString &clientName);

    QString purpose() const;
    void setPurpose(const QString &purpose);

    qreal amountPaid() const;
    void setAmountPaid(qreal amountPaid);
signals:
    void clientNameChanged();
    void purposeChanged();
    void amountPaidChanged();
public slots:
    void push() override;
protected:
    void processResult(const QueryResult result) override;
private:
    QString m_clientName;
    QString m_purpose;
    qreal m_amountPaid;
};

#endif // QMLINCOMEPUSHER_H
