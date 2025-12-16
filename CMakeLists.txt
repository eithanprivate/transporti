cmake_minimum_required(VERSION 3.18)

project(Transporti VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt5 packages (compatible with Debian 11)
find_package(Qt5 5.15 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    3DCore
    3DRender
    3DInput
    3DExtras
    Network
    Concurrent
)

# Zstandard compression
if(WIN32)
    # Windows: use vcpkg's find_package
    find_package(zstd CONFIG REQUIRED)
    set(ZSTD_LIBRARIES $<IF:$<TARGET_EXISTS:zstd::libzstd_shared>,zstd::libzstd_shared,zstd::libzstd_static>)
    set(ZSTD_INCLUDE_DIRS "")
    set(ZSTD_LIBRARY_DIRS "")
else()
    # Linux/macOS: use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ZSTD REQUIRED libzstd)
endif()

# Platform-specific settings
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DPLATFORM_LINUX)
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${ZSTD_INCLUDE_DIRS}
)

# Add subdirectories
add_subdirectory(src)

# Qt resource file
qt5_add_resources(QML_RESOURCES resources/qml.qrc)

# Main executable
add_executable(Transporti
    src/main.cpp
    ${QML_RESOURCES}
)

# Link libraries
target_link_libraries(Transporti PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::3DCore
    Qt5::3DRender
    Qt5::3DInput
    Qt5::3DExtras
    Qt5::Network
    Qt5::Concurrent
    ${ZSTD_LIBRARIES}
    TransportiLib
)

# Link Zstandard library directories (only for Linux/macOS with pkg-config)
if(ZSTD_LIBRARY_DIRS)
    target_link_directories(Transporti PRIVATE ${ZSTD_LIBRARY_DIRS})
endif()

# Installation
install(TARGETS Transporti
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# Platform-specific packaging
if(WIN32)
    # Windows installer
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_NAME "Transporti")
    set(CPACK_PACKAGE_VENDOR "Transporti")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PC Migration Assistant")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "Transporti")
elseif(UNIX)
    # Linux packages
    set(CPACK_GENERATOR "DEB;RPM")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Transporti Team")
    set(CPACK_PACKAGE_DESCRIPTION "PC Migration Assistant for transferring data from Linux to Windows")
endif()

include(CPack)
