#ifndef MESSAGECENTER_H
#define MESSAGECENTER_H

#include <QObject>

class MessageCenter : public QObject
{
    Q_OBJECT
public:
    explicit MessageCenter(QObject* parent = nullptr);

    QVariant getMessage(const QString& key) const;
};

#endif  // MESSAGECENTER_H
