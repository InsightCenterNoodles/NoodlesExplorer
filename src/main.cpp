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
    QCoreApplication::setOrganizationName("NREL");
    QCoreApplication::setOrganizationDomain("nrel.gov");
    QCoreApplication::setApplicationName("NoodlesExplorer");
    QCoreApplication::setApplicationVersion("0.4");

    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create("Fusion"));

    QFontDatabase::addApplicationFont(":/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fa-solid-900.ttf");

    // Create models

    State state;


    // Open Engine

    QQmlApplicationEngine engine;

    QUrl const url(QStringLiteral("qrc:/qml/main.qml"));

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
