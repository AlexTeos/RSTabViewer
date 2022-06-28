#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "musiclibrary.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MusicLibrary library(QDir(""));
    engine.rootContext()->setContextProperty("songs", &library);
    engine.rootContext()->setContextProperty("tablature", library.tablature());

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}
