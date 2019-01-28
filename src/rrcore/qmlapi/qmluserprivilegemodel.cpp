#include "qmluserprivilegemodel.h"

#include "models/userprivilegemodel.h"
#include "database/databaseexception.h"

QMLUserPrivilegeModel::QMLUserPrivilegeModel(QObject *parent) :
    AbstractVisualListModel (parent),
    m_privilegeModels(QList<UserPrivilegeModel *>()),
    m_userId(-1)
{

}

QMLUserPrivilegeModel::QMLUserPrivilegeModel(DatabaseThread &thread) :
    AbstractVisualListModel (thread),
    m_privilegeModels(QList<UserPrivilegeModel *>()),
    m_userId(-1)
{

}

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
        return QVariant::fromValue(m_privilegeModels.at(index.row()));
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
    if (groupIndex < 0 || groupIndex >= m_privilegeModels.count()) {
        qWarning() << "Out of range group index detected!";
        return;
    }

    UserPrivilegeModel *model = m_privilegeModels.at(groupIndex);
    if (privilegeIndex < 0 || privilegeIndex >= model->rowCount()) {
        qWarning() << "Out of range privilege index detected!";
        return;
    }

    model->setPrivilegeValue(privilegeIndex, value);
}

bool QMLUserPrivilegeModel::submit()
{
    if (m_userName.trimmed().isEmpty() && m_userId == -1) {
        emit error(NoUserNameSetError);
    } else if (m_firstName.trimmed().isEmpty() && m_userId == -1) {
        emit error(NoFirstNameSetError);
    } else if (m_lastName.trimmed().isEmpty() && m_userId == -1) {
        emit error(NoLastNameSetError);
    } else if (m_password.trimmed().isEmpty() && m_userId == -1) {
        emit error(NoPasswordSetError);
    } else if (m_phoneNumber.trimmed().isEmpty() && m_userId == -1) {
        emit error(NoPhoneNumberSetError);
    } else if (m_emailAddress.trimmed().isEmpty() && m_userId == -1) {
        emit error(NoEmailAddressSetError);
    } else {
        QVariantMap groups;
        for (auto model : m_privilegeModels) {
            groups.insert(model->privilegeGroup(),
                          QVariantMap {
                              { "title", model->title() },
                              { "privileges", model->privileges() },
                          });
        }

        QueryRequest request(this);
        QVariantMap params;
        params.insert("user_privileges", groups);

        if (m_userId > -1) {
            params.insert("user_id", m_userId);
            request.setCommand("update_user_privileges", params, QueryRequest::User);
        } else {
            params.insert("first_name", m_firstName);
            params.insert("last_name", m_lastName);
            params.insert("user_name", m_userName);
            params.insert("image_url", m_imageUrl);
            params.insert("password", m_password);
            params.insert("phone_number", m_phoneNumber);
            params.insert("email_address", m_emailAddress);
            request.setCommand("add_user", params, QueryRequest::User);
        }

        emit executeRequest(request);
        return true;
    }

    return false;
}

void QMLUserPrivilegeModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_user_privileges", { { "user_id", m_userId } }, QueryRequest::User);
    emit executeRequest(request);
}

void QMLUserPrivilegeModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == "view_user_privileges") {
            beginResetModel();
            QMapIterator<QString, QVariant> mapIter(result.outcome().toMap().value("user_privileges").toMap());
            while (mapIter.hasNext()) {
                auto pair = mapIter.next();

                UserPrivilegeModel *model = new UserPrivilegeModel(this);
                QVariantMap privilegeDetails = pair.value().toMap();
                model->setPrivileges(pair.key(), privilegeDetails.value("title").toString(), privilegeDetails.value("privileges").toList());

                m_titles.append(privilegeDetails.value("title").toString());
                m_privilegeGroups.append(pair.key());
                m_privilegeModels.append(model);
            }

            endResetModel();
            emit success();
        } else if (result.request().command() == "add_user") {
            emit success(AddUserSuccess);
        } else if (result.request().command() == "update_user_privileges") {
            emit success(UpdateUserSuccess);
        }
    } else {
        switch (result.errorCode()) {
        case static_cast<int>(DatabaseException::RRErrorCode::DuplicateEntryFailure):
        case static_cast<int>(DatabaseException::RRErrorCode::CreateUserFailed):
            emit error(UserAlreadyExistsError);
            break;
        case static_cast<int>(DatabaseException::RRErrorCode::UserPreviouslyArchived):
            emit error(UserPreviouslyArchivedError);
            break;
        default:
            emit error();
        }
    }
}
