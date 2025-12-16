#include "WindowsUtils.h"
#include <QStandardPaths>

QString WindowsUtils::getUserDirectory() {
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}
