#ifndef QMLDATABASECREATOR_H
#define QMLDATABASECREATOR_H

#include <QObject>
#include <QFutureWatcher>

class DatabaseCreator;

class QMLDatabaseCreator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
public:
    explicit QMLDatabaseCreator(QObject *parent = nullptr);

    bool busy() const;
    Q_INVOKABLE void start();
signals:
    void success();
    void error();
    void busyChanged();
private:
    bool m_busy;
    DatabaseCreator *m_databaseCreator;
    QFutureWatcher<bool> m_futureWatcher;

    void setBusy(bool busy);
    void onFinished();
};

#endif // QMLDATABASECREATOR_H
