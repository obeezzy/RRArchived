#include "abstractreportmodel.h"
#include "database/databasethread.h"

AbstractReportModel::AbstractReportModel(QObject *parent) :
    AbstractReportModel(DatabaseThread::instance(), parent)
{

}

AbstractReportModel::AbstractReportModel(DatabaseThread &thread,
                                         QObject *parent) :
    AbstractVisualTableModel(thread, parent),
    m_dateTimeSpan(QDateTime(QDate(QDate::currentDate().year(), 1, 1), QTime(0, 0)),
                   QDateTime::currentDateTime())
{

}

QDateTime AbstractReportModel::from() const
{
    return m_dateTimeSpan.from;
}

void AbstractReportModel::setFrom(const QDateTime &from)
{
    if (m_dateTimeSpan.from == from)
        return;

    m_dateTimeSpan.from = from;
    emit fromChanged();
}

QDateTime AbstractReportModel::to() const
{
    return m_dateTimeSpan.to;
}

void AbstractReportModel::setTo(const QDateTime &to)
{
    if (m_dateTimeSpan.to == to)
        return;

    m_dateTimeSpan.to = to;
    emit toChanged();
}

Utility::DateTimeSpan AbstractReportModel::dateTimeSpan() const
{
    return m_dateTimeSpan;
}
