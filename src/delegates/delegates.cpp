#include "delegates.h"

#include <noo_common.h>

QStringList to_qstringlist(std::vector<std::string_view> v) {
    QStringList ret;

    for (auto s : v) {
        ret << noo::to_qstring(s);
    }

    return ret;
}
