#ifndef NOTE_H
#define NOTE_H

#include <QVariantMap>
#include <QString>

namespace Utility {
struct Note {
    int id {-1};
    QString note;

    explicit Note() = default;
    explicit Note(int id,
                  const QString &note);
    explicit Note(const QString &note);
    explicit Note(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};
}
Q_DECLARE_TYPEINFO(Utility::Note, Q_PRIMITIVE_TYPE);

#endif // NOTE_H
