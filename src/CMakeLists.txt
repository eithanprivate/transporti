# Core library
add_library(TransportiLib STATIC
    # Core
    core/Application.cpp
    core/ApplicationState.cpp

    # Network
    network/NetworkManager.cpp
    network/Discovery/DiscoveryService.cpp
    network/Discovery/PeerInfo.cpp
    network/Transfer/TransferProtocol.cpp
    network/Transfer/TransferClient.cpp
    network/Transfer/TransferServer.cpp
    network/Transfer/PacketManager.cpp
    network/Security/PairingManager.cpp

    # Scanner
    scanner/ScannerBase.cpp
    scanner/LinuxScanner/LinuxDataScanner.cpp
    scanner/LinuxScanner/DocumentScanner.cpp
    scanner/LinuxScanner/ConfigScanner.cpp
    scanner/LinuxScanner/ApplicationScanner.cpp
    scanner/LinuxScanner/EmailScanner.cpp
    scanner/LinuxScanner/BrowserScanner.cpp

    # Compression
    compression/CompressionEngine.cpp
    compression/ZstdCompressor.cpp

    # Models
    models/PeerListModel.cpp
    models/DataCategoryModel.cpp
    models/TransferProgressModel.cpp

    # Platform
    platform/PlatformUtils.cpp
    platform/LinuxUtils.cpp
    platform/WindowsUtils.cpp
)

target_link_libraries(TransportiLib PUBLIC
    Qt5::Core
    Qt5::Network
    Qt5::Qml
    Qt5::Concurrent
    ${ZSTD_LIBRARIES}
)

target_include_directories(TransportiLib PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${ZSTD_INCLUDE_DIRS}
)
