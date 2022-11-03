#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "logger.h"
#include "musiclibrary.h"

int main(int argc, char* argv[])
{
    qInstallMessageHandler(Logger::myMessageOutput);
    qInfo() << "Start application";

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

#ifdef Q_OS_ANDROID
    QDir workDirectory("/sdcard/RSTabsData/");
#else
    QDir workDirectory("./RSTabsData/");
#endif

    MusicLibrary library(workDirectory);
    engine.rootContext()->setContextProperty("songs", &library);
    engine.rootContext()->setContextProperty("tablature", library.tablature());

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}
