#ifndef QMLUSERPROFILE_H
#define QMLUSERPROFILE_H

#include <QObject>

class QueryRequest;
class QueryResult;
class DatabaseThread;

class QMLUserProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(bool isFirstTime READ isFirstTime CONSTANT)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY adminChanged)
public:
    explicit QMLUserProfile(QObject *parent = nullptr);
    explicit QMLUserProfile(DatabaseThread &);

    enum ErrorCode {
        Unknown,
        NoUserNameProvided,
        NoPasswordProvided,
        InvalidUserName,
        IncorrectCredentials
    }; Q_ENUM(ErrorCode)

    bool isBusy() const;
    void setBusy(bool busy);

    bool isFirstTime() const;

    bool isAdmin() const;

    Q_INVOKABLE void signIn(const QString &userName, const QString &password);
    Q_INVOKABLE void signInOnline(const QString &emailAddress, const QString &password);
    Q_INVOKABLE void signUp(const QString &userName, const QString &password);

    Q_INVOKABLE bool hasPrivilege(const QString &privilege);
signals:
    void executeRequest(const QueryRequest &);
    void busyChanged();
    void success();
    void error(int errorCode);

    void isFirstTimeChanged();
    void adminChanged();
private slots:
    void processResult(const QueryResult &);
private:
    bool m_busy;
    bool m_admin;

    void setAdmin(bool admin);
};

#endif // QMLUSERPROFILE_H
