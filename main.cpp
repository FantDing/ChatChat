#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include"socket.h"
#include<QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    
    Socket* socket=new Socket();
    engine.rootContext()->setContextObject(socket);
    
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}