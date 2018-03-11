#include "qmlcommandmodel.h"

QMLCommandModel::QMLCommandModel(QAbstractListModel *parent) :
    m_type(Misc),
    QAbstractListModel(parent)
{

}

QString QMLCommandModel::command() const
{
    return m_command;
}

void QMLCommandModel::setCommand(const QString &command)
{
    if (m_command == command)
        return;

    m_command = command;
    emit commandChanged();
}

QMLCommandModel::QueryType QMLCommandModel::type() const
{
    return m_type;
}

void QMLCommandModel::setType(QueryType type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged();
}
