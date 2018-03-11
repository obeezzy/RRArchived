#ifndef BACKUPTHREAD_H
#define BACKUPTHREAD_H

#include <QThread>

class BackupThread : public QThread
{
public:
    explicit BackupThread(QObject *parent = nullptr);
};

#endif // BACKUPTHREAD_H
