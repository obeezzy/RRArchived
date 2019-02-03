#ifndef QMLRECEIPTPRINTER_H
#define QMLRECEIPTPRINTER_H

#include <QObject>
#include <QSharedPointer>

class QQuickItem;
class QQuickItemGrabResult;

class QMLReceiptPrinter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy WRITE setBusy NOTIFY busyChanged)
public:
    explicit QMLReceiptPrinter(QObject *parent = nullptr);

    bool isBusy() const;
    void setBusy(bool busy);

    Q_INVOKABLE void print(const QString &job);
signals:
    void busyChanged();
private:
    bool m_busy;
    QSharedPointer<QQuickItemGrabResult> m_result;

    QQuickItem *createReceipt(const QString &job);
};

#endif // QMLRECEIPTPRINTER_H
