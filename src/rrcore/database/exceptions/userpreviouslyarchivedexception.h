#ifndef USERPREVIOUSLYARCHIVEDEXCEPTION_H
#define USERPREVIOUSLYARCHIVEDEXCEPTION_H

#include "database/databaseexception.h"

class UserPreviouslyArchivedException : public DatabaseException
{
public:
    explicit UserPreviouslyArchivedException(
        const QString& message = QString());
    QString toString() const override;
};

#endif  // USERPREVIOUSLYARCHIVEDEXCEPTION_H
