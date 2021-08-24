#include "state.h"

#include <QApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStyle>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include <tuple>

int main(int argc, char* argv[]) {

#if __APPLE__
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setVersion(4, 1);

    QSurfaceFormat::setDefaultFormat(fmt);
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QCoreApplication::setOrganizationName("NREL");
    QCoreApplication::setOrganizationDomain("nrel.gov");
    QCoreApplication::setApplicationName("NoodlesExplorer");

    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create("Fusion"));

    QFontDatabase::addApplicationFont(":/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fa-solid-900.ttf");

    // Create models

    State state;


    // Open Engine

    QQmlApplicationEngine engine;

    QUrl const url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // register models
    state.link(engine.rootContext());


    engine.load(url);

    qDebug() << "Setup complete, running...";

    return app.exec();
}
