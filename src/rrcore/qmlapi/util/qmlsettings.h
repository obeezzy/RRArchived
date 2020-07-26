#ifndef QMLSETTINGS_H
#define QMLSETTINGS_H

#include <QObject>
#include <QSettings>

class QMLSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool darkModeActive READ darkModeActive WRITE setDarkModeActive
                   NOTIFY darkModeActiveChanged)
public:
    explicit QMLSettings(QObject* parent = nullptr);

    bool darkModeActive() const;
    void setDarkModeActive(bool darkModeActive);
signals:
    void darkModeActiveChanged();

private:
    QSettings m_settings;
};

#endif  // QMLSETTINGS_H
