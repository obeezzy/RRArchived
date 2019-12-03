#include "adduser.h"

using namespace UserQuery;

AddUser::AddUser(const QString &firstName,
                 const QString &lastName,
                 const QString &userName,
                 const QString &password,
                 const QString &phoneNumber,
                 const QString &emailAddress,
                 const QUrl &imageUrl,
                 QObject *receiver) :
    UserExecutor(COMMAND, {
                        { "first_name", firstName },
                        { "last_name", lastName },
                        { "user_name", userName },
                        { "password", password },
                        { "phone_number", phoneNumber },
                        { "email_address", emailAddress },
                        { "image_source", imageUrl }
                 }, receiver)
{

}

QueryResult AddUser::execute()
{
    QueryResult result;

    return result;
}
