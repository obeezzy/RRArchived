#include "note.h"
#include <QVariantMap>

using namespace Utility;

Note::Note(int id, const QString& note) : id(id), note(note) {}

Note::Note(const QString& note) : id(-1), note(note) {}

Note::Note(const QVariantMap& map)
    : id(map.value("note_id").toInt()),
      note(map.value("note").toString())
{}

QVariantMap Note::toVariantMap() const
{
    return {{"note_id", id}, {"note", note}};
}
