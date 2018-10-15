#ifndef QMLDATABASECREATOR_H
#define QMLDATABASECREATOR_H

#include <QObject>
#include <QFutureWatcher>

class DatabaseCreator;

class QMLDatabaseCreator : public QObject
{
    Q_OBJECT
public:
    explicit QMLDatabaseCreator(QObject *parent = nullptr);

    Q_INVOKABLE void start();
signals:
    void databaseCreated();
    void errorOccurred();
private:
    DatabaseCreator *m_databaseCreator;
    QFutureWatcher<bool> m_futureWatcher;

    void onFinished();
};

#endif // QMLDATABASECREATOR_H
