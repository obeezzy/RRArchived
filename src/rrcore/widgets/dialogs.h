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

static QObject *dialogs_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Dialogs *dialogs = new Dialogs();
    return dialogs;
}

#endif // DIALOGS_H
