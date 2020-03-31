#include "qmluserprivilegemodel.h"
#include "database/databaseerror.h"
#include "database/databasethread.h"
#include "queryexecutors/user.h"
#include "utility/commonutils.h"
#include "utility/userutils.h"

QMLUserPrivilegeModel::QMLUserPrivilegeModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

QMLUserPrivilegeModel::QMLUserPrivilegeModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel (thread, parent)
{}

int QMLUserPrivilegeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_privilegeGroups.count();
}

QVariant QMLUserPrivilegeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case PrivilegeGroupRole:
        return m_privilegeGroups.at(index.row());
    case TitleRole:
        return m_titles.at(index.row());
    case PrivilegeModelRole:
        return QVariant();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLUserPrivilegeModel::roleNames() const
{
    return {
        { PrivilegeGroupRole, "privilege_group" },
        { TitleRole, "title" },
        { PrivilegeModelRole, "privilege_model" }
    };
}

bool QMLUserPrivilegeModel::isExistingUser() const
{
    return m_userId > 0;
}

int QMLUserPrivilegeModel::userId() const
{
    return m_userId;
}

void QMLUserPrivilegeModel::setUserId(int userId)
{
    if (m_userId == userId)
        return;

    m_userId = userId;
    emit userIdChanged();
}

QString QMLUserPrivilegeModel::firstName() const
{
    return m_firstName;
}

void QMLUserPrivilegeModel::setFirstName(const QString &firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged();
}

QString QMLUserPrivilegeModel::lastName() const
{
    return m_lastName;
}

void QMLUserPrivilegeModel::setLastName(const QString &lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged();
}

QString QMLUserPrivilegeModel::userName() const
{
    return m_userName;
}

void QMLUserPrivilegeModel::setUserName(const QString &userName)
{
    if (m_userName == userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}

QString QMLUserPrivilegeModel::password() const
{
    return m_password;
}

void QMLUserPrivilegeModel::setPassword(const QString &password)
{
    if (m_password == password)
        return;

    m_password = password;
    emit passwordChanged();
}

QUrl QMLUserPrivilegeModel::imageUrl() const
{
    return m_imageUrl;
}

void QMLUserPrivilegeModel::setImageUrl(const QUrl &imageUrl)
{
    if (m_imageUrl == imageUrl)
        return;

    m_imageUrl = imageUrl;
    emit imageUrlChanged();
}

QString QMLUserPrivilegeModel::phoneNumber() const
{
    return m_phoneNumber;
}

void QMLUserPrivilegeModel::setPhoneNumber(const QString &phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

QString QMLUserPrivilegeModel::emailAddress() const
{
    return m_emailAddress;
}

void QMLUserPrivilegeModel::setEmailAddress(const QString &emailAddress)
{
    if (m_emailAddress == emailAddress)
        return;

    m_emailAddress = emailAddress;
    emit emailAddressChanged();
}

void QMLUserPrivilegeModel::setPrivilegeValue(int groupIndex, int privilegeIndex, bool value)
{
    Q_UNUSED(groupIndex)
    Q_UNUSED(privilegeIndex)
    Q_UNUSED(value)
}

bool QMLUserPrivilegeModel::submit()
{
    if (m_userName.trimmed().isEmpty() && !isExistingUser()) {
        emit error(ModelResult{ NoUserNameSetError });
    } else if (m_firstName.trimmed().isEmpty() && !isExistingUser()) {
        emit error(ModelResult{ NoFirstNameSetError });
    } else if (m_lastName.trimmed().isEmpty() && !isExistingUser()) {
        emit error(ModelResult{ NoLastNameSetError });
    } else if (m_password.trimmed().isEmpty() && !isExistingUser()) {
        emit error(ModelResult{ NoPasswordSetError });
    } else if (m_phoneNumber.trimmed().isEmpty() && !isExistingUser()) {
        emit error(ModelResult{ NoPhoneNumberSetError });
    } else if (m_emailAddress.trimmed().isEmpty() && !isExistingUser()) {
        emit error(ModelResult{ NoEmailAddressSetError });
    } else {
        if (isExistingUser()) {
            emit execute(new UserQuery::UpdateUserPrivileges(m_userId,
                                                             Utility::UserPrivilegeList { },
                                                             this));
        } else {
            emit execute(new UserQuery::AddUser(Utility::User {
                                                    m_firstName,
                                                    m_lastName,
                                                    m_userName,
                                                    m_password,
                                                    m_phoneNumber,
                                                    m_emailAddress,
                                                    m_imageUrl
                                                },
                                                this));
        }

        return true;
    }

    return false;
}

void QMLUserPrivilegeModel::tryQuery()
{
    setBusy(true);
    emit execute(new UserQuery::FetchUserPrivileges(m_userId,
                                                    this));
}

bool QMLUserPrivilegeModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLUserPrivilegeModel::processResult(const QueryResult &result)
{
    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == UserQuery::FetchUserPrivileges::COMMAND) {
            beginResetModel();
            // Missing implementation
            endResetModel();
            emit success();
        } else if (result.request().command() == UserQuery::AddUser::COMMAND) {
            emit success(ModelResult{ AddUserSuccess });
        } else if (result.request().command() == UserQuery::UpdateUserPrivileges::COMMAND) {
            emit success(ModelResult{ UpdateUserSuccess });
        }
    } else {
        switch (result.errorCode()) {
        case DatabaseError::asInteger(DatabaseError::QueryErrorCode::DuplicateEntryFailure):
        case DatabaseError::asInteger(DatabaseError::QueryErrorCode::CreateUserFailed):
            emit error(ModelResult{ UserAlreadyExistsError });
            break;
        case DatabaseError::asInteger(DatabaseError::QueryErrorCode::UserPreviouslyArchived):
            emit error(ModelResult{ UserPreviouslyArchivedError });
            break;
        default:
            emit error();
        }
    }
}
