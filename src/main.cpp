#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "core/Application.h"
#include "core/ApplicationState.h"
#include "models/PeerListModel.h"
#include "models/DataCategoryModel.h"
#include "models/TransferProgressModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Application info
    app.setOrganizationName("Transporti");
    app.setApplicationName("Transporti PC Migration");
    app.setApplicationVersion("1.0.0");

    // Create core application
    Application* coreApp = new Application(&app);

    // QML engine
    QQmlApplicationEngine engine;

    // Register C++ types for QML
    qmlRegisterType<PeerListModel>("Transporti.Models", 1, 0, "PeerListModel");
    qmlRegisterType<DataCategoryModel>("Transporti.Models", 1, 0, "DataCategoryModel");
    qmlRegisterType<TransferProgressModel>("Transporti.Models", 1, 0, "TransferProgressModel");
    qmlRegisterUncreatableType<ApplicationState>("Transporti.Core", 1, 0, "ApplicationState", "Cannot create ApplicationState");

    // Register enums for QML
    qRegisterMetaType<ApplicationState::AppRole>("ApplicationState::AppRole");
    qRegisterMetaType<ApplicationState::AppStage>("ApplicationState::AppStage");

    // Expose models to QML
    engine.rootContext()->setContextProperty("appCore", coreApp);
    engine.rootContext()->setContextProperty("appState", coreApp->appState());
    engine.rootContext()->setContextProperty("peerModel", coreApp->peerModel());
    engine.rootContext()->setContextProperty("categoryModel", coreApp->categoryModel());
    engine.rootContext()->setContextProperty("progressModel", coreApp->progressModel());

    // Load main QML
    const QUrl url(QStringLiteral("qrc:/qt/qml/Transporti/qml/main.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML";
        return -1;
    }

    return app.exec();
}
