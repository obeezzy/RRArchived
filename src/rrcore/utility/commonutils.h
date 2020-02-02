#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QString>
#include <QVariantMap>

struct Note {
    int id;
    QString note;

    explicit Note() : id(-1) { }
    explicit Note(int id, const QString &note) :
        id(id), note(note) { }
    explicit Note(const QString &note) :
        id(-1), note(note) { }
    explicit Note(const QVariantMap &map) :
        id(map.value("note_id").toInt()),
        note(map.value("note").toString()) { }

    QVariantMap toVariantMap() const {
        return {
            { "note_id", id },
            { "note", note }
        };
    }
};

struct RelatedTransaction {
    int id;
    QString tableName;

    explicit RelatedTransaction() :
        id(-1) { }
    explicit RelatedTransaction(int id, const QString &tableName) :
        id(id), tableName(tableName) { }
};

#endif // COMMONUTILS_H
