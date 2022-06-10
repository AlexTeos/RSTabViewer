#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "tablature.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    RS::SNG sng;
    sng.parse("stro1251_lead_uncompressed.sng");
    Tablature tablature;
    tablature.m_sng = sng;
    tablature.collectAllNotes();

    engine.rootContext()->setContextProperty("tablature", &tablature);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}
