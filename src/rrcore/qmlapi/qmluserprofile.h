#ifndef QMLUSERPROFILE_H
#define QMLUSERPROFILE_H

#include <QObject>

class QueryRequest;
class QueryResult;

class QMLUserProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy WRITE setBusy NOTIFY busyChanged)
public:
    explicit QMLUserProfile(QObject *parent = nullptr);

    enum ErrorCode {
        Unknown,
        NoUserNameProvided,
        IncorrectCredentials
    }; Q_ENUM(ErrorCode)

    bool isBusy() const;
    void setBusy(bool busy);

    Q_INVOKABLE void signIn(const QString &userName, const QString &password);
    Q_INVOKABLE void signUp(const QString &userName, const QString &password);
signals:
    void requestSent(const QueryRequest &);
    void busyChanged();
    void success();
    void error(int errorCode);
private slots:
    void processResult(const QueryResult &);
private:
    bool m_busy;
};

#endif // QMLUSERPROFILE_H
