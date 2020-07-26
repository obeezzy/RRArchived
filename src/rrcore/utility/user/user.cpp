#include "user.h"
#include <QString>
#include "utility/common/recordtimestamp.h"

using namespace Utility::User;

User::User(const QVariantMap& map)
    : id(map.value("user_id").toInt()),
      user(map.value("username").toString()),
      password(map.value("password").toString()),
      firstName(map.value("first_name").toString()),
      lastName(map.value("last_name").toString()),
      imageUrl(map.value("image_url").toUrl()),
      phoneNumber(map.value("phone_number").toString()),
      emailAddress(map.value("email_address").toString()),
      preset(map.value("preset").toString()),
      privileges(UserPrivilegeList{map.value("user_privileges").toList()}),
      flags((map.value("active").toBool() ? RecordGroup::Active
                                          : RecordGroup::None) |
            (map.value("archived").toBool() ? RecordGroup::Archived
                                            : RecordGroup::None)),
      note(Note{map}),
      timestamp(RecordTimestamp{map}),
      accessToken(map.value("access_token").toByteArray())
{
    Q_UNUSED(map)
}

User::User(const QString& user, const QString& password)
    : user(user),
      password(password)
{}

User::User(int id, const QString& user) : id(id), user(user) {}

User::User(int id, const QString& user, const QString& firstName,
           const QString& lastName, const QUrl& imageUrl,
           const QString& phoneNumber, const QString& emailAddress,
           const RecordGroup::Flags& flags, const Note& note)
    : id(id),
      user(user),
      firstName(firstName),
      lastName(lastName),
      imageUrl(imageUrl),
      phoneNumber(phoneNumber),
      emailAddress(emailAddress),
      flags(flags),
      note(note)
{}

User::User(const QString& firstName, const QString& lastName,
           const QString& user, const QString& password,
           const QString& phoneNumber, const QString& emailAddress,
           const QUrl& imageUrl)
    : user(user),
      password(password),
      firstName(firstName),
      lastName(lastName),
      imageUrl(imageUrl),
      phoneNumber(phoneNumber),
      emailAddress(emailAddress)
{}

QVariantMap User::toVariantMap() const
{
    return {{"user_id", id},
            {"username", user},
            {"password", password},
            {"first_name", firstName},
            {"last_name", lastName},
            {"image_url", imageUrl},
            {"phone_number", phoneNumber},
            {"email_address", emailAddress},
            {"preset", preset},
            {"privileges", privileges.toVariantList()},
            {"active", flags.testFlag(RecordGroup::Active)},
            {"archived", flags.testFlag(RecordGroup::Archived)},
            {"note_id", note.id},
            {"note", note.note}};
}
