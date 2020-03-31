#ifndef ABSTRACTKPIMODEL_H
#define ABSTRACTKPIMODEL_H

#include "abstractvisualtablemodel.h"
#include "utility/common/datetimespan.h"
#include <QObject>

class AbstractKpiModel : public AbstractVisualTableModel
{
    Q_OBJECT
    Q_PROPERTY(QDateTime from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QDateTime to READ to WRITE setTo NOTIFY toChanged)
public:
    explicit AbstractKpiModel(QObject *parent = nullptr);
    explicit AbstractKpiModel(DatabaseThread &thread,
                              QObject *parent = nullptr);
    ~AbstractKpiModel() = default;

    QDateTime from() const;
    void setFrom(const QDateTime &from);

    QDateTime to() const;
    void setTo(const QDateTime &to);
protected:
    Utility::DateTimeSpan dateTimeSpan() const;
signals:
    void fromChanged();
    void toChanged();
private:
    Utility::DateTimeSpan m_dateTimeSpan;
};

#endif // ABSTRACTKPIMODEL_H
