#ifndef RECORDGROUP_H
#define RECORDGROUP_H

#include <QFlags>

namespace Utility {
inline namespace Common {
class RecordGroup
{
public:
    enum Flag
    {
        None = (1 << 0),
        Archived = (1 << 1),
        Suspended = (1 << 2),
        Tracked = (1 << 3),
        Divisible = (1 << 4),
        Active = (1 << 5),
        AcceptsCash = (1 << 6),
        AcceptsCard = (1 << 7),
        Preferred = (1 << 8)
    };
    Q_DECLARE_FLAGS(Flags, Flag)

private:
    explicit RecordGroup() = default;
};
}  // namespace Common
}  // namespace Utility
Q_DECLARE_OPERATORS_FOR_FLAGS(Utility::RecordGroup::Flags)

#endif  // RECORDGROUP_H
