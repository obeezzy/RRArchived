#ifndef ABSTRACTREPORTMODEL_H
#define ABSTRACTREPORTMODEL_H

#include <QDateTime>
#include "models/abstractvisualtablemodel.h"
#include "utility/common/datetimespan.h"

class DatabaseThread;

class AbstractReportModel : public AbstractVisualTableModel
{
    Q_OBJECT
    Q_PROPERTY(QDateTime from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QDateTime to READ to WRITE setTo NOTIFY toChanged)
public:
    explicit AbstractReportModel(QObject* parent = nullptr);
    explicit AbstractReportModel(DatabaseThread& thread,
                                 QObject* parent = nullptr);

    QDateTime from() const;
    void setFrom(const QDateTime& from);

    QDateTime to() const;
    void setTo(const QDateTime& to);
signals:
    void fromChanged();
    void toChanged();

protected:
    Utility::DateTimeSpan dateTimeSpan() const;

private:
    Utility::DateTimeSpan m_dateTimeSpan;
};

#endif  // ABSTRACTREPORTMODEL_H
