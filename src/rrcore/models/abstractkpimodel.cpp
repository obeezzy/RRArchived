#include "abstractkpimodel.h"
#include "database/databasethread.h"

AbstractKpiModel::AbstractKpiModel(QObject *parent) :
    AbstractKpiModel(DatabaseThread::instance(), parent)
{}

AbstractKpiModel::AbstractKpiModel(DatabaseThread &thread,
                                   QObject *parent) :
    AbstractVisualTableModel(thread, parent)
{}

QDateTime AbstractKpiModel::from() const
{
    return m_dateTimeSpan.from;
}

void AbstractKpiModel::setFrom(const QDateTime &from)
{
    if (m_dateTimeSpan.from == from)
        return;

    m_dateTimeSpan.from = from;
    emit fromChanged();
}

QDateTime AbstractKpiModel::to() const
{
    return m_dateTimeSpan.to;
}

void AbstractKpiModel::setTo(const QDateTime &to)
{
    if (m_dateTimeSpan.to == to)
        return;

    m_dateTimeSpan.to = to;
    emit toChanged();
}

Utility::DateTimeSpan AbstractKpiModel::dateTimeSpan() const
{
    return m_dateTimeSpan;
}
