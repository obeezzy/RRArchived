#ifndef DIALOGS_H
#define DIALOGS_H

#include <QObject>

class QQmlEngine;
class QJSEngine;

class Dialogs : public QObject
{
    Q_OBJECT
public:
    explicit Dialogs(QObject *parent = nullptr);

public slots:
    void showPrintDialog();
};

static QObject *dialogs_provider(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    Dialogs *dialogs = new Dialogs();
    return dialogs;
}

#endif // DIALOGS_H
