# Instalación en Debian 11 (Bullseye)

## Nota Importante

Debian 11 no tiene Qt6 en sus repositorios, por lo que el proyecto ha sido adaptado para usar **Qt5**, que funciona perfectamente para Transporti.

## Paquetes Instalados

```bash
sudo apt install -y qtbase5-dev qtdeclarative5-dev qt3d5-dev \
                    qtquickcontrols2-5-dev libzstd-dev cmake \
                    build-essential qml-module-qtquick-controls2 \
                    qml-module-qt-labs-settings \
                    qml-module-qtgraphicaleffects
```

## Compilación

```bash
cd /home/eithancrea/Desktop/transporti
rm -rf build
mkdir build
cd build
cmake ..
make -j4
```

## Ejecutar

```bash
./Transporti
```

## Diferencias con Qt6

El proyecto fue adaptado de Qt6 a Qt5 con los siguientes cambios:

1. **CMakeLists.txt**: Cambiado `find_package(Qt6` a `find_package(Qt5`
2. **Imports QML**: Cambiados de `import QtQuick` a `import QtQuick 2.15`
3. **GraphicalEffects**: Cambiado de `Qt5Compat.GraphicalEffects` a `QtGraphicalEffects`
4. **main.cpp**: Removido `objectCreationFailed` (no existe en Qt5)

## Estado del Proyecto

✅ **Compilación exitosa** - 100%
✅ **Ejecutable creado** - 281KB
✅ **Todas las librerías compiladas correctamente**

El proyecto está listo para ejecutarse en Debian 11!
