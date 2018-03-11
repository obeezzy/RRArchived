#ifndef QMLCOMMANDMODEL_H
#define QMLCOMMANDMODEL_H

#include <QAbstractListModel>

class QMLCommandModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
    Q_PROPERTY(QueryType type READ type WRITE setType NOTIFY typeChanged)
    Q_ENUMS(QueryType)
public:
    explicit QMLCommandModel(QAbstractListModel *parent = nullptr);

    enum QueryType {
        Misc = -1,
        User,
        Stock,
        Sales,
        Purchase,
        Income,
        Expenses
    };

    QString command() const;
    void setCommand(const QString &);

    QueryType type() const;
    void setType(QueryType);
signals:
    void commandChanged();
    void typeChanged();
private:
    QString m_command;
    QueryType m_type;
};

#endif // QMLCOMMANDMODEL_H
