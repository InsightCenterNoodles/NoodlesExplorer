#include "delegates.h"

#include "instance_material/qinstancedmetalroughmaterial.h"
#include "tabledata.h"

#include <glm/gtc/type_ptr.hpp>

#include <QColor>
#include <QDebug>
#include <QTreeWidgetItem>

#include <QAbstractLight>
#include <QAttribute>
#include <QEntity>
#include <QGeometry>
#include <QGeometryRenderer>
#include <QMaterial>


#include <Qt3DRender/QBuffer>


#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QMetalRoughMaterial>

#include <QFile>

QStringList to_qstringlist(std::vector<std::string_view> v) {
    QStringList ret;

    for (auto s : v) {
        ret << noo::to_qstring(s);
    }

    return ret;
}
