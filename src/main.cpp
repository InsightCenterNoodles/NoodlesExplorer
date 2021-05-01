#include "state.h"

#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
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
    QCoreApplication::setApplicationName("NoodlesViewer");

    QGuiApplication app(argc, argv);

    QFontDatabase::addApplicationFont(":/Roboto-Regular.ttf");

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

    return app.exec();
}
