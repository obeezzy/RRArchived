#include "viewclients.h"

using namespace ClientQuery;

ViewClients::ViewClients(QObject *receiver) :
    ClientExecutor(COMMAND, { }, receiver)
{

}

ViewClients::ViewClients(const QString &filterText, const QString &filterColumn, QObject *receiver) :
    ClientExecutor(COMMAND, {
                        { "filter_text", filterText },
                        { "filter_column", filterColumn }
                   }, receiver)
{

}

QueryResult ViewClients::execute()
{
    QueryResult result{ request() };

    return result;
}
