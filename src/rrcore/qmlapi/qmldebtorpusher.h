#ifndef QMLDEBTORPUSHER_H
#define QMLDEBTORPUSHER_H

#include <QObject>
#include "pusher/abstractpusher.h"

class QMLDebtorPusher : public AbstractPusher
{
    Q_OBJECT
public:
    explicit QMLDebtorPusher(QObject *parent = nullptr);
protected:
    void processResult(const QueryResult &result) override final;
public slots:
    void push() override final;
};

#endif // QMLDEBTORPUSHER_H
