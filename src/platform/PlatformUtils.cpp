#include "PlatformUtils.h"

QString PlatformUtils::getPlatformName() {
#ifdef PLATFORM_LINUX
    return "Linux";
#elif defined(PLATFORM_WINDOWS)
    return "Windows";
#else
    return "Unknown";
#endif
}
