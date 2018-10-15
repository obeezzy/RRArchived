#ifndef QMLUSERPROFILE_H
#define QMLUSERPROFILE_H

#include <QObject>

class QueryRequest;
class QueryResult;

class QMLUserProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(bool isFirstTime READ isFirstTime CONSTANT)
public:
    explicit QMLUserProfile(QObject *parent = nullptr);

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

    Q_INVOKABLE void signIn(const QString &userName, const QString &password);
    Q_INVOKABLE void signInOnline(const QString &emailAddress, const QString &password);
    Q_INVOKABLE void signUp(const QString &userName, const QString &password);
    Q_INVOKABLE void removeUser(const QString &userName);
signals:
    void executeRequest(const QueryRequest &);
    void busyChanged();
    void success();
    void error(int errorCode);

    void isFirstTimeChanged();
private slots:
    void processResult(const QueryResult &);
private:
    bool m_busy;
};

#endif // QMLUSERPROFILE_H
