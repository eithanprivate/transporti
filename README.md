# Transporti - PC Migration Assistant

A beautiful native desktop application for migrating data from Linux to Windows, featuring a Mac OS X Aqua-inspired skeuomorphic UI with stunning 3D cube transfer animations.

![Transporti](docs/screenshot.png)

## Features

- **Automatic PC Discovery** - Automatically finds devices on your local network
- **Comprehensive Data Migration** - Transfer documents, configs, applications, emails, and browser data
- **Real-time Compression** - Uses Zstandard compression for fast, efficient transfers
- **Beautiful Aqua UI** - Skeuomorphic design inspired by classic Mac OS X
- **3D Animations** - Dynamic cube animations visualize your data transfer in real-time
- **Secure Pairing** - 6-digit PIN ensures only authorized connections
- **Progress Tracking** - Visual sidebar shows your migration progress

## Technology Stack

- **Qt 6.5+** with QML for native cross-platform UI
- **Qt Quick 3D** for hardware-accelerated 3D cube animations
- **Zstandard** compression for optimal transfer speed
- **C++17** backend with Qt Network for robust data transfer
- **CMake** build system

## Requirements

### Linux (Source PC)
- Qt 6.5 or higher
- libzstd development libraries
- CMake 3.18+
- C++17 compatible compiler (GCC 7+, Clang 5+)
- Linux distribution with dpkg, rpm, or pacman package manager

### Windows (Destination PC)
- Qt 6.5 or higher
- libzstd libraries
- CMake 3.18+
- Visual Studio 2019+ or MinGW

## Installation

### Ubuntu/Debian (Linux)
```bash
# Install Qt6 and dependencies
sudo apt update
sudo apt install qt6-base-dev qt6-declarative-dev qt6-quick3d-dev \
                 libzstd-dev cmake build-essential git

# Clone the repository
cd ~/Desktop/transporti

# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# Run
./build/Transporti
```

### Fedora/RHEL (Linux)
```bash
# Install Qt6 and dependencies
sudo dnf install qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qt3d-devel \
                 libzstd-devel cmake gcc-c++ git

# Build and run (same as Ubuntu)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/Transporti
```

### Arch Linux
```bash
# Install Qt6 and dependencies
sudo pacman -S qt6-base qt6-declarative qt6-quick3d \
               zstd cmake base-devel git

# Build and run
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/Transporti
```

### Windows
```powershell
# Install Qt6 from official website: https://www.qt.io/download
# Install Visual Studio 2022 Community Edition

# Install vcpkg for dependencies
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install zstd:x64-windows

# Build Transporti
cd path\to\transporti
cmake -B build -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release

# Run
.\build\Release\Transporti.exe
```

## Usage

### Quick Start

1. **On both computers**: Install and launch Transporti

2. **Select roles**:
   - On your old Linux PC: Select "Source (Linux)"
   - On your new Windows PC: Select "Destination (Windows)"

3. **Connect**: Transporti will automatically discover devices on your local network. Select the destination PC from the list.

4. **Pair devices**: Verify the 6-digit PIN code matches on both screens and confirm.

5. **Scan data**: Transporti scans your Linux PC for transferable data.

6. **Select categories**: Choose which data categories to transfer:
   - ✅ Documents (~/Documents, ~/Downloads, ~/Desktop, ~/Pictures, ~/Videos, ~/Music)
   - ✅ Configurations (application settings)
   - ✅ Applications (package list for manual reinstallation)
   - ✅ Emails (Thunderbird, Evolution)
   - ✅ Browser Data (Firefox, Chrome bookmarks and settings)

7. **Transfer**: Watch the beautiful 3D cube animation as your data transfers!

8. **Complete**: Your data is now on your Windows PC, organized in the appropriate folders.

## Project Structure

```
transporti/
├── CMakeLists.txt                 # Root build configuration
├── README.md                      # This file
├── src/                           # C++ backend
│   ├── main.cpp
│   ├── core/                      # Application core
│   ├── network/                   # Network layer (discovery, transfer)
│   ├── scanner/                   # Data scanning
│   ├── compression/               # Zstandard compression
│   ├── models/                    # Qt models for QML
│   └── platform/                  # Platform-specific utilities
├── qml/                           # QML UI
│   ├── main.qml
│   ├── components/                # Reusable Aqua-styled components
│   ├── views/                     # Application views
│   ├── animations/                # 3D cube animations
│   ├── styles/                    # Theme system
│   └── layout/                    # Layout components
└── resources/                     # Images, icons, shaders
```

## Development

### Building for Development
```bash
# Debug build with symbols
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
./build-debug/Transporti
```

### Code Style
- C++: Follow Qt coding conventions
- QML: Use declarative patterns, avoid imperative code
- Indentation: 4 spaces (no tabs)
- Line length: 100 characters max

### Contributing
Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Commit your changes with clear messages
4. Submit a pull request

## Architecture

### Network Protocol
- **Discovery**: UDP broadcast on port 47623
- **Transfer**: TCP on port 47624
- **Packet Format**: Binary protocol with CRC32 checksums
- **Compression**: Zstandard level 3 for optimal speed/ratio

### UI Design
- **Skeuomorphic Aqua Theme**: Glossy buttons, gradients, drop shadows
- **Color Palette**: White-to-blue gradient background (#FFFFFF → #E8F2FF)
- **3D Engine**: Qt Quick 3D with PrincipledMaterial for glass-like cubes
- **Animations**: Smooth transitions with easing curves

## Roadmap

### Phase 1: Foundation ✅
- [x] Project structure
- [x] CMake build system
- [x] Aqua UI theme
- [x] Basic views and navigation

### Phase 2: Network (In Progress)
- [x] UDP discovery service
- [x] Peer detection
- [ ] TCP transfer protocol
- [ ] Pairing mechanism

### Phase 3: Data Scanning (Next)
- [ ] Document scanner
- [ ] Config scanner
- [ ] Application scanner
- [ ] Email scanner
- [ ] Browser data scanner

### Phase 4: Transfer
- [ ] File transfer client/server
- [ ] Zstandard compression
- [ ] Progress tracking
- [ ] Error handling

### Phase 5: 3D Visualization
- [ ] Qt Quick 3D scene
- [ ] Animated data cubes
- [ ] Performance optimization

### Phase 6: Polish
- [ ] Windows support
- [ ] Cross-platform testing
- [ ] Installers (DEB, RPM, NSIS)
- [ ] User documentation

## Troubleshooting

### "No devices found"
- Ensure both computers are on the same local network
- Check firewall settings (ports 47623 and 47624)
- Try disabling VPN temporarily

### "Transfer failed"
- Check available disk space on destination
- Verify network connection is stable
- Check file permissions on source files

### Build errors
- Ensure Qt 6.5+ is installed: `qmake --version`
- Verify CMake version: `cmake --version` (need 3.18+)
- Check zstd installation: `pkg-config --modversion libzstd`

## License

MIT License - See LICENSE file for details

## Credits

Created with ❤️ using Qt, QML, and Qt Quick 3D

Inspired by the timeless design of Mac OS X Aqua interface
