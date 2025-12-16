#include "LinuxUtils.h"
#include <QStandardPaths>

QString LinuxUtils::getHomeDirectory() {
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}
