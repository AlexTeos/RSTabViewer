#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "musiclibrary.h"
#include "tablature.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MusicLibrary library(QDir(""));
    engine.rootContext()->setContextProperty("songs", &library);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}
